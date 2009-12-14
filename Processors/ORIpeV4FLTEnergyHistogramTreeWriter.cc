// ORIpeV4FLTEnergyHistogramTreeWriter.cc

#include "ORIpeV4FLTEnergyHistogramTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORIpeV4FLTEnergyHistogramTreeWriter::ORIpeV4FLTEnergyHistogramTreeWriter(string treeName) :
ORVTreeWriter(new ORIpeV4FLTEnergyHistogramDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORIpeV4FLTEnergyHistogramDecoder*>(fDataDecoder);
  Clear();
}

ORIpeV4FLTEnergyHistogramTreeWriter::~ORIpeV4FLTEnergyHistogramTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORIpeV4FLTEnergyHistogramTreeWriter::InitializeBranches()
{
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  
  fTree->Branch("readoutSec", &fReadoutSec, "readoutSec/i");
  fTree->Branch("refreshTime", &fRefreshTime, "refreshTime/i");
  fTree->Branch("firstBin", &fFirstBin, "firstBin/i");
  fTree->Branch("lastBin", &fLastBin, "lastBin/i");
  fTree->Branch("histogramLength", &fHistogramLength, "histogramLength/i");
  fTree->Branch("maxHistogramLength", &fMaxHistogramLength, "maxHistogramLength/i");
  fTree->Branch("binSize", &fBinSize, "binSize/i");
  fTree->Branch("offsetEMin", &fOffsetEMin, "offsetEMin/i");
  fTree->Branch("histogramID", &fHistID, "histogramID/i");
  fTree->Branch("histogramInfo", &fHistInfo, "histogramInfo/i");
  fTree->Branch("histogram", fHistogram, "histogram[histogramLength]/i");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORIpeV4FLTEnergyHistogramTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
      // check severity to improve speed:
  fCrate = fEventDecoder->CrateOf(record);
  fCard = fEventDecoder->CardOf(record);
  fChannel = fEventDecoder->ChannelOf(record);

  fReadoutSec = fEventDecoder->ReadoutSecOf(record);
  fRefreshTime = fEventDecoder->RefreshTimeOf(record);
  fFirstBin = fEventDecoder->FirstBinOf(record);
  fLastBin = fEventDecoder->LastBinOf(record);
  fHistogramLength = fEventDecoder->HistogramLengthOf(record);
  fMaxHistogramLength = fEventDecoder->MaxHistogramLengthOf(record);
  fBinSize = fEventDecoder->BinSizeOf(record);
  fOffsetEMin = fEventDecoder->OffsetEMinOf(record);
  fHistID =  fEventDecoder->HistogramIDOf(record);
  fHistInfo =  fEventDecoder->HistogramInfoOf(record);

  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "crate-card-channel-fReadoutSec-fRefreshTime-fFirstBin-fLastBin-fHistogramLength-fMaxHistogramLength-fBinSize-fOffsetEMin-fID-fInfo: "
      << fCrate << "-" << fCard << "-" << fChannel  << "-"
      << fReadoutSec << "-" << fRefreshTime << "-" << fFirstBin << "-" << fLastBin << "-" << fHistogramLength << "-" << fMaxHistogramLength << "-" << fBinSize << "-" << fOffsetEMin
      << "-" << fHistID << "-" <<  fHistInfo
      << endl;
  }
  if(fHistogramLength > kMaxHistoLength) {
    ORLog(kError) << "Histogram length (" << fHistogramLength 
      << ") exceeds kMaxHistoLength (" << kMaxHistoLength << ")" << endl;
    return kFailure;
  }
  
  fEventDecoder->CopyHistogramData( fHistogram, kMaxHistoLength );

  return kSuccess;
}


