// ORAugerFLTWaveformTreeWriter.cc

#include "ORAugerFLTWaveformTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORAugerFLTWaveformTreeWriter::ORAugerFLTWaveformTreeWriter(string treeName) :
ORVTreeWriter(new ORAugerFLTWaveformDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORAugerFLTWaveformDecoder*>(fDataDecoder);
  Clear();
  fWaveform[0] = 0;  // this is on the stack, not the heap.
}

ORAugerFLTWaveformTreeWriter::~ORAugerFLTWaveformTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORAugerFLTWaveformTreeWriter::InitializeBranches()
{
  fTree->Branch("wfLength", &fWaveformLength, "wfLength/I");
  fTree->Branch("eventSec", &fSec, "eventSec/i");
  fTree->Branch("eventSubSec", &fSubSec, "eventSubSec/i");
  fTree->Branch("eventID", &fEventID, "eventSubSec/i");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("energy", &fEnergy, "energy/i");
  fTree->Branch("waveform", fWaveform, "waveform[wfLength]/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORAugerFLTWaveformTreeWriter::ProcessMyDataRecord(UInt_t* record)
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
  fWaveformLength = fEventDecoder->GetWaveformLen();
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

