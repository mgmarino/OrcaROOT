// ORKatrinV4FLTEnergyTreeWriter.cc

#include "ORKatrinV4FLTEnergyTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORKatrinV4FLTEnergyTreeWriter::ORKatrinV4FLTEnergyTreeWriter(string treeName) :
ORVTreeWriter(new ORKatrinV4FLTEnergyDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORKatrinV4FLTEnergyDecoder*>(fDataDecoder);
  Clear();
}

ORKatrinV4FLTEnergyTreeWriter::~ORKatrinV4FLTEnergyTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORKatrinV4FLTEnergyTreeWriter::InitializeBranches()
{
  fTree->Branch("eventSec", &fSec, "eventSec/i");
  fTree->Branch("eventSubSec", &fSubSec, "eventSubSec/i");
  //fTree->Branch("resetSec", &fResetSec, "resetSec/i");//removed 2011-06-14 -tb-
  //fTree->Branch("resetSubSec", &fResetSubSec, "resetSubSec/i");//removed 2011-06-14 -tb-
  fTree->Branch("fifoEventID", &fEventID, "fifoEventID/s");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("energy_adc", &fEnergy, "energy_adc/i");
  fTree->Branch("channelMap", &fChannelMap, "channelMap/s");
  fTree->Branch("pageNumber", &fPageNumber, "pageNumber/s");
  fTree->Branch("eventInfo", &fEventInfo, "eventInfo/i");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORKatrinV4FLTEnergyTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
      // check severity to improve speed:
  fSec = fEventDecoder->SecondsOf(record);
  fSubSec = fEventDecoder->SubSecondsOf(record);
  //fResetSec = fEventDecoder->ResetSecondsOf(record);//removed 2011-06-14 -tb-
  //fResetSubSec = fEventDecoder->ResetSubSecondsOf(record);//removed 2011-06-14 -tb-
  fEnergy = fEventDecoder->EnergyOf(record);
  fCrate = fEventDecoder->CrateOf(record);
  fCard = fEventDecoder->CardOf(record);
  fChannel = fEventDecoder->ChannelOf(record);
  fChannelMap = fEventDecoder->ChannelMapOf(record);
  fPageNumber = fEventDecoder->PageNumberOf(record);
  fEventID = fEventDecoder->EventIDOf(record);
  fEventInfo =  fEventDecoder->EventInfoOf(record);//TODO: not yet in kDebug output, see below -tb-
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "sec-subsec-ID-crate-card-channel-energy_adc = "
      << fSec << "-" << fSubSec << "-" 
      << fEventID << "-"
	  << fCrate << "-"
      << fCard << "-" << fChannel << "-" << fEnergy << endl;
  }
  return kSuccess;
}


