// ORAcqirisDC440TreeWriter.cc

#include "ORAcqirisDC440TreeWriter.hh"
#include "ORLogger.hh"
#include <sstream> 
#include "ORRunContext.hh"
#include "ORDictionary.hh"

using namespace std;

ORAcqirisDC440TreeWriter::ORAcqirisDC440TreeWriter(string treeName) :
ORVTreeWriter(new ORAcqirisDC440Decoder, treeName)
{
  fEventDecoder = dynamic_cast<ORAcqirisDC440Decoder*>(fDataDecoder);
  fEventTime = 0.;
  fCrate = 0;
  fCard = 0;
  fChannel = 0;
  fWaveformLength = 0;
  SetDoNotAutoFillTree();
}

ORAcqirisDC440TreeWriter::~ORAcqirisDC440TreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORAcqirisDC440TreeWriter::InitializeBranches()
{
  fTree->Branch("wfLength", &fWaveformLength, "wfLength/i");
  fTree->Branch("EventTime", &fEventTime, "LEDEventTime/D");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("waveform", fWaveform, "waveform[wfLength]/s");
  fTree->Branch("samplingPeriod", &fSamplingPeriod, "samplingPeriod/D");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORAcqirisDC440TreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
  fEventTime = fEventDecoder->GetTimeStamp();
  fCrate = fEventDecoder->CrateOf();
  fCard = fEventDecoder->CardOf();
  fChannel = fEventDecoder->GetChannelNum();
  fWaveformLength = fEventDecoder->GetWaveformLen();
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "time-crate-card-channel-length- = "
      << fEventTime << "-"
      << fCrate << "-"
      << fCard << "-" 
      << fChannel << "-"
      << fWaveformLength << "-"
      << endl;
  }
  ostringstream os;
  os << "crate " << fCrate-1 << ":station " << fCard+1;
  const ORDictionary* dict = NULL;
  if (fRunContext) {
    dict = dynamic_cast<const ORDictionary*>(fRunContext->GetHeader()->LookUp(os.str()));
  }
  if(!dict) {
    ORLog(kError) << "Header corrupt!" << endl;
    return kFailure;
  }

  const ORDictValueR* sampleInterval = dynamic_cast<const ORDictValueR*>(dict->LookUp("sampleInterval")); 
  const ORDictValueR* delayTime = dynamic_cast<const ORDictValueR*>(dict->LookUp("delayTime")); 
  if(!sampleInterval || !delayTime) {
    ORLog(kError) << "sample or delay time not found!" << endl;
    return kFailure;
  }
  fSamplingPeriod = sampleInterval->GetR();
  if (fWaveformLength > kMaxWFLength) {
    ORLog(kError) << "Waveform too long for kMaxWFLength!" << endl;
    return kFailure;
  }
  fEventDecoder->CopyWaveformData(fWaveform, fWaveformLength);
  fTree->Fill();
  return kSuccess;
}

