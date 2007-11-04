// ORBocTIC3TreeWriter.cc

#include "ORBocTIC3TreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORBocTIC3TreeWriter::ORBocTIC3TreeWriter(string treeName) :
ORVTreeWriter(new ORBocTIC3Decoder, treeName)
{
  fBocTIC3Decoder = dynamic_cast<ORBocTIC3Decoder*>(fDataDecoder); 
  Clear();
  SetDoNotAutoFillTree();
}

ORBocTIC3TreeWriter::~ORBocTIC3TreeWriter()
{
  delete fBocTIC3Decoder;
}

ORDataProcessor::EReturnCode ORBocTIC3TreeWriter::InitializeBranches()
{
  fTree->Branch("pressure", &fPressure, "temp/F");
  fTree->Branch("time", &fTime, "time/i");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("deviceID", &fDeviceID, "deviceID/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORBocTIC3TreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  fDeviceID = fBocTIC3Decoder->GetDeviceID(record);
  for (UInt_t i=0; i<fBocTIC3Decoder->GetNumberOfChannels();i++) {
    fPressure = fBocTIC3Decoder->GetPressureOfChannel(record, i);
    fTime = fBocTIC3Decoder->GetTimeOfChannel(record, i);
    fChannel = i;
    if(ORLogger::GetSeverity() >= ORLogger::kDebug) { 
      ORLog(kDebug) << "channel:pressure:time = "  
        << fChannel << ":" << fPressure << ":" 
        << ":" << fTime << endl;
    }
    fTree->Fill();
  }

  return kSuccess;
}

