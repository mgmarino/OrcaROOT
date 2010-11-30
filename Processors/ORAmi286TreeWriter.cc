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
  fTree->Branch("status",&fStatus, "status/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORAmi286TreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  fDeviceID = fAmi286Decoder->GetDeviceID(record);
  for (UInt_t i=0; i<fAmi286Decoder->GetNumberOfChannels();i++) {
    fLevel = fAmi286Decoder->GetLevelOfChannel(record, i);
    fTime = fAmi286Decoder->GetTimeOfChannel(record, i);
    fStatus = fAmi286Decoder->GetStatusOfChannel(record,i);
    fChannel = i;
    if(ORLogger::GetSeverity() >= ORLogger::kDebug) { 
      ORLog(kDebug) << std::endl << "   Channel: " << fChannel << std::endl
      << "     Level: " << fLevel << std::endl
      << "     Time: " << fTime << std::endl
      << "     Status: " << fStatus << std::endl;
    }
    fTree->Fill();
  }

  return kSuccess;
}

