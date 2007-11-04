// ORDGF4cEnergyTreeWriter.cc

#include "ORDGF4cEnergyTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORDGF4cEnergyTreeWriter::ORDGF4cEnergyTreeWriter(string treeName) :
ORVTreeWriter(new ORDGF4cEventDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORDGF4cEventDecoder*>(fDataDecoder);
  fEventTime = 0;
  fCrate = 0;
  fCard = 0;
  fChannel = 0;
  fEnergy = 0;
  SetDoNotAutoFillTree();
}

ORDGF4cEnergyTreeWriter::~ORDGF4cEnergyTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORDGF4cEnergyTreeWriter::InitializeBranches()
{
  fTree->Branch("eventTime", &fEventTime, "eventTime/D");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("energy", &fEnergy, "energy/S");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORDGF4cEnergyTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  fEventDecoder->SetDataRecord(record);
  for (size_t i = 0; i < fEventDecoder->GetNEvents(); i++) {
    for (size_t k = 0; k < fEventDecoder->GetNChannels(i); k++) {
      // check severity to improve speed:
      if (ORLogger::GetSeverity() >= ORLogger::kDebug) { 
        ORLog(kDebug) << "ProcessMyDataRecord(): " 
                      << "event-time-crate-card-channel-energy = "
                      << i << "-" 
                      << fEventDecoder->GetEventTime(i) << "-"
                      << fEventDecoder->CrateOf() << "-"
                      << fEventDecoder->CardOf() << "-" 
                      << k << "-"
                      << fEventDecoder->GetChanEnergy(i,k) << endl;
      }
      fEventTime = fEventDecoder->GetEventTime(i);
      fCrate = fEventDecoder->CrateOf();
      fCard = fEventDecoder->CardOf();
      fChannel = k;
      fEnergy = fEventDecoder->GetChanEnergy(i,k);
      fTree->Fill();
    }
  }
  return kSuccess;
}

