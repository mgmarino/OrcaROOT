// ORKatrinFLTEnergyTreeWriter.cc

#include "ORKatrinFLTEnergyTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORKatrinFLTEnergyTreeWriter::ORKatrinFLTEnergyTreeWriter(string treeName) :
ORVTreeWriter(new ORKatrinFLTEnergyDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORKatrinFLTEnergyDecoder*>(fDataDecoder);
  Clear();
}

ORKatrinFLTEnergyTreeWriter::~ORKatrinFLTEnergyTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORKatrinFLTEnergyTreeWriter::InitializeBranches()
{
  fTree->Branch("eventSec", &fSec, "eventSec/i");
  fTree->Branch("eventSubSec", &fSubSec, "eventSubSec/i");
  fTree->Branch("eventID", &fEventID, "eventID/i");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("energy", &fEnergy, "energy/i");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORKatrinFLTEnergyTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
      // check severity to improve speed:
  fSec = fEventDecoder->SecondsOf(record);
  fSubSec = fEventDecoder->SubSecondsOf(record);
  fEnergy = fEventDecoder->EnergyOf(record);
  fCrate = fEventDecoder->CrateOf(record);
  fCard = fEventDecoder->CardOf(record);
  fChannel = fEventDecoder->ChannelOf(record);
  fEventID = fEventDecoder->EventIDOf(record);
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "sec-subsec-ID-crate-card-channel-energy = "
      << fSec << "-" << fSubSec << "-" << fCrate << "-"
      << fEventID << "-"
      << fCard << "-" << fChannel << "-" << fEnergy << endl;
  }
  return kSuccess;
}

