// ORAmi286TreeWriter.cc

#include "ORAmi286TreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORAmi286TreeWriter::ORAmi286TreeWriter(string treeName) :
ORVTreeWriter(new ORAmi286Decoder, treeName)
{
  fAmi286Decoder = dynamic_cast<ORAmi286Decoder*>(fDataDecoder); 
  Clear();
  SetDoNotAutoFillTree();
}

ORAmi286TreeWriter::~ORAmi286TreeWriter()
{
  delete fAmi286Decoder;
}

ORDataProcessor::EReturnCode ORAmi286TreeWriter::InitializeBranches()
{
  fTree->Branch("level", &fLevel, "level/F");
  fTree->Branch("time", &fTime, "time/i");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("deviceID", &fDeviceID, "deviceID/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORAmi286TreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  fDeviceID = fAmi286Decoder->GetDeviceID(record);
  for (UInt_t i=0; i<fAmi286Decoder->GetNumberOfChannels();i++) {
    fLevel = fAmi286Decoder->GetLevelOfChannel(record, i);
    fTime = fAmi286Decoder->GetTimeOfChannel(record, i);
    fChannel = i;
    if(ORLogger::GetSeverity() >= ORLogger::kDebug) { 
      ORLog(kDebug) << "channel:level:time = "  
        << fChannel << ":" << fLevel << ":" 
        << ":" << fTime << endl;
    }
    fTree->Fill();
  }

  return kSuccess;
}

