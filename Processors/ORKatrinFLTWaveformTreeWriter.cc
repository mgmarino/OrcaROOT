// ORKatrinFLTWaveformTreeWriter.cc

#include "ORKatrinFLTWaveformTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORKatrinFLTWaveformTreeWriter::ORKatrinFLTWaveformTreeWriter(string treeName) :
ORVTreeWriter(new ORKatrinFLTWaveformDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORKatrinFLTWaveformDecoder*>(fDataDecoder);
  Clear();
  fWaveform[0] = 0;  // this is on the stack, not the heap.
}

ORKatrinFLTWaveformTreeWriter::~ORKatrinFLTWaveformTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORKatrinFLTWaveformTreeWriter::InitializeBranches()
{
  fTree->Branch("wfLength", &fWaveformLength, "wfLength/I");
  fTree->Branch("eventSec", &fSec, "eventSec/i");
  fTree->Branch("eventSubSec", &fSubSec, "eventSubSec/i");
  fTree->Branch("eventID", &fEventID, "eventID/s");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("channelMap", &fChannelMap, "channelMap/i");
  fTree->Branch("pageNumber", &fPageNumber, "pageNumber/s");
  fTree->Branch("energy", &fEnergy, "energy/i");
  fTree->Branch("resetSec", &fResetSec, "resetSec/i");
  fTree->Branch("resetSubSec", &fResetSubSec, "resetSubSec/i");
  fTree->Branch("waveform", fWaveform, "waveform[wfLength]/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORKatrinFLTWaveformTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
      // check severity to improve speed:
  fSec = fEventDecoder->GetSec();
  fSubSec = fEventDecoder->GetSubSec();
  fEventID = fEventDecoder->GetEventID();
  fEnergy = fEventDecoder->GetEnergy();
  fCrate = fEventDecoder->CrateOf();
  fCard = fEventDecoder->CardOf();
  fChannel = fEventDecoder->GetChannel();
  fChannelMap = fEventDecoder->GetChannelMap();
  fPageNumber = fEventDecoder->GetPageNumber();
  fWaveformLength = fEventDecoder->GetWaveformLen();
  fResetSec = fEventDecoder->GetResetSec();
  fResetSubSec = fEventDecoder->GetResetSubSec();
   
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "event-sec-subsec-crate-card-channel-energy = "
      << fEventID << "-" << fSec << "-" << fSubSec << "-" << fCrate << "-"
      << fCard << "-" << fChannel << "-" << fEnergy << endl;
  }
  if(fWaveformLength > kMaxWFLength) {
    ORLog(kError) << "Waveform length (" << fWaveformLength 
      << ") exceeds kMaxWFLength (" << kMaxWFLength << ")" << endl;
    return kFailure;
  }
  memcpy(fWaveform, fEventDecoder->GetWaveformDataPointer(), 
    fWaveformLength*sizeof(UShort_t));
  return kSuccess;
}

