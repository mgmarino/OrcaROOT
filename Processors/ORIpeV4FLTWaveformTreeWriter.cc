// ORIpeV4FLTWaveformTreeWriter.cc

#include "ORIpeV4FLTWaveformTreeWriter.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

using namespace std;

ORIpeV4FLTWaveformTreeWriter::ORIpeV4FLTWaveformTreeWriter(string treeName) :
ORVTreeWriter(new ORIpeV4FLTWaveformDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORIpeV4FLTWaveformDecoder*>(fDataDecoder);
  Clear();
  fWaveform[0] = 0;  // this is on the stack, not the heap.
}

ORIpeV4FLTWaveformTreeWriter::~ORIpeV4FLTWaveformTreeWriter()
{
  delete fEventDecoder;
}

/** Create the ROOT branches.
  * 
  */ //-tb- 2008-02-12
ORDataProcessor::EReturnCode ORIpeV4FLTWaveformTreeWriter::InitializeBranches()
{
  fTree->Branch("wfLength", &fWaveformLength, "wfLength/i");
  fTree->Branch("eventSec", &fSec, "eventSec/i");
  fTree->Branch("eventSubSec", &fSubSec, "eventSubSec/i");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("channelMap", &fChannelMap, "channelMap/i");
  fTree->Branch("eventID", &fEventID, "eventID/i");
  fTree->Branch("energy_adc", &fEnergy, "energy_adc/i");
  fTree->Branch("eventFlags", &fEventFlags, "eventFlags/i");
  fTree->Branch("eventInfo", &fEventInfo, "eventInfo/i");
  fTree->Branch("waveform", fWaveform, "waveform[wfLength]/s");
  return kSuccess;
}

/**  This sets the data/variables for the ROOT branches.
  *  TTree::Fill() is called in ORBasicTreeWriter::ProcessMyDataRecord(UInt_t* record) (?)
  *  This class is a ORDataProcessor and ORVTreeWriter, NOT a ORBasicTreeWriter!
  *  (See comments in ORDataProcessor.hh and ORVTreeWriter.hh)
  */ //-tb- 2008-02-12
ORDataProcessor::EReturnCode ORIpeV4FLTWaveformTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
      // check severity to improve speed:
  fCrate = fEventDecoder->CrateOf();
  fCard = fEventDecoder->CardOf();
  fChannel = fEventDecoder->GetChannel();
  fChannelMap = fEventDecoder->GetChannelMap();
  fSec = fEventDecoder->GetSec();
  fSubSec = fEventDecoder->GetSubSec();
  fEventID = fEventDecoder->GetEventID();
  fEnergy = fEventDecoder->GetEnergy();
  fWaveformLength = fEventDecoder->GetWaveformLen();
  fEventFlags = fEventDecoder->GetEventFlags();
  fEventInfo = fEventDecoder->GetEventInfo();
   
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "crate-card-channel-sec-subsec-energy_adc-chmap-eventID-eventFlags-eventInfo = "
	  << fCrate << "-"  << fCard << "-" << fChannel << "-" 
	  << fSec << "-" << fSubSec << "-" << fEnergy 
	  << "-" << fChannelMap 
      << "-" << fEventID << "-" <<  fEventFlags  //-tb- 2010-02-16
      << "-" << fEventInfo  //-tb- 2010-02-16
      << endl;
  }
  if(fWaveformLength > kMaxWFLength) {
    ORLog(kError) << "Waveform length (" << fWaveformLength 
      << ") exceeds kMaxWFLength (" << kMaxWFLength << ")" << endl;
    return kFailure;
  }
  
  fEventDecoder->CopyWaveformData( fWaveform, kMaxWFLength );

  return kSuccess;
}


