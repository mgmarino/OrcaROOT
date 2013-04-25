// ORKatrinV4FLTHitrateTreeWriter.cc

#include "ORKatrinV4FLTHitRateTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORKatrinV4FLTHitRateTreeWriter::ORKatrinV4FLTHitRateTreeWriter(string treeName) :
ORVTreeWriter(new ORKatrinV4FLTHitRateDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORKatrinV4FLTHitRateDecoder*>(fDataDecoder);
  Clear();
}

//ORKatrinV4FLTHitRateTreeWriter::~ORKatrinV4FLTHitRateTreeWriter()
//{
//  delete fEventDecoder;
//}

ORDataProcessor::EReturnCode ORKatrinV4FLTHitRateTreeWriter::InitializeBranches()
{
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("version", &fVersion, "version/s");
  fTree->Branch("second", &fSecond, "second/i");
  fTree->Branch("hitrateLength", &fHitrateLength, "hitrateLength/i");
  fTree->Branch("totalHitrate", &fTotalHitrate, "totalHitrate/i");
  fTree->Branch("NChannels", &fNChannels, "NChannels/i");
  fTree->Branch("rawData16", fRawData16, "rawData16[NChannels]/i");
  fTree->Branch("rawData32", fRawData32, "rawData32[NChannels]/i");
  return kSuccess;

}

ORDataProcessor::EReturnCode ORKatrinV4FLTHitRateTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
      // check severity to improve speed:
  fCrate = fEventDecoder->CrateOf(record);
  fCard = fEventDecoder->CardOf(record);
  fVersion = fEventDecoder->VersionOf(record);
  fSecond = fEventDecoder->SecondsOf(record);

  fHitrateLength = fEventDecoder->HitRateLengthOf(record);
  fTotalHitrate = fEventDecoder->TotalHitRateOf(record);
  //fChannel = fEventDecoder->ChannelOf(record);
  fNChannels = fEventDecoder->NChannelsOf(record);

  //fill arrays
  fEventDecoder->CopyRawData(fRawData16,fRawData32);
  
  
  
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "sec-fVersion-crate-card-totalRate-NChannels = "
      << fSecond << "-" << fVersion << "-" 
	  << fCrate << "-"
      << fCard << "-" 
      << fTotalHitrate << "-" 
      << fNChannels << endl;
  }
  return kSuccess;
}


