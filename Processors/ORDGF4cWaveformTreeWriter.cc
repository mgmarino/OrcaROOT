// ORDGF4cWaveformTreeWriter.cc

#include "ORDGF4cWaveformTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORDGF4cWaveformTreeWriter::ORDGF4cWaveformTreeWriter(string treeName) :
ORVTreeWriter(new ORDGF4cEventDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORDGF4cEventDecoder*>(fDataDecoder);
  fEventTime = 0;
  fCrate = 0;
  fCard = 0;
  fChannel = 0;
  fEnergy = 0;
  fWaveformLength = 0;
  fWaveform[0] = 0;  // this is on the stack, not the heap.
  SetDoNotAutoFillTree();
}

ORDGF4cWaveformTreeWriter::~ORDGF4cWaveformTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORDGF4cWaveformTreeWriter::InitializeBranches()
{
  fTree->Branch("wfLength", &fWaveformLength, "wfLength/I");
  fTree->Branch("eventTime", &fEventTime, "eventTime/D");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("energy", &fEnergy, "energy/s");
  fTree->Branch("waveform", fWaveform, "waveform[wfLength]/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORDGF4cWaveformTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
  ORLog(kDebug) << "Found events: " << fEventDecoder->GetDGFNEvents() << endl;
  for (size_t i = 0; i < fEventDecoder->GetDGFNEvents(); i++) 
  {
    for (size_t k = 0; k < fEventDecoder->GetNChannels(i); k++) 
    {
      // check severity to improve speed:
      ORLog(kDebug) << "Found channels: " << fEventDecoder->GetNChannels(i)
          << " in event: " << i  << endl;
      if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
      { 
        ORLog(kDebug) << "ProcessMyDataRecord(): "
          << "event-time-crate-card-channel-energy = "
          << i << "-" 
          << fEventDecoder->GetEventTime(i) << "-"
          << fEventDecoder->CrateOf() << "-"
          << fEventDecoder->CardOf() << "-" 
          << k << "-"
          << fEventDecoder->GetChanEnergy(i,k) << endl;
      }
    fEventTime = fEventDecoder->GetDGFEventTime(i);
    fCrate = fEventDecoder->CrateOf();
    fCard = fEventDecoder->CardOf();
    fChannel = fEventDecoder->GetChannelNumber(i,k);
    fEnergy = fEventDecoder->GetChanEnergy(i,k);
    fWaveformLength = (Int_t) fEventDecoder->CopyWaveformData(fWaveform, kMaxWFLength, i, k);
    fTree->Fill();
    }
  }
  return kSuccess;
}

