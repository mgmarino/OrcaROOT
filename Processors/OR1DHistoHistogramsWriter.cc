// OR1DHistoHistogramsWriter.cc

#include "OR1DHistoHistogramsWriter.hh"
#include "ORLogger.hh"
#include "TH1.h"

using namespace std;

OR1DHistoHistogramsWriter::OR1DHistoHistogramsWriter(string histogramName) :
ORDataProcessor(new OR1DHistoHistogramsDecoder)
{
  f1DHistoHistogramsDecoder = dynamic_cast<OR1DHistoHistogramsDecoder*>(fDataDecoder);
  fHistNameBase = histogramName;
}

ORDataProcessor::EReturnCode OR1DHistoHistogramsWriter::ProcessMyDataRecord(UInt_t* record)
{
  string histName = fHistNameBase;
  int iHist=1;
  while(gDirectory->Get(histName.data()) != NULL) {
    histName = Form("%s%02d", fHistNameBase.data(), iHist);
    iHist++;
  }

  string histTitle;
  for(size_t iKey = 0; iKey < f1DHistoHistogramsDecoder->NKeysOf(record); iKey++) {
    if(iKey != 0) histTitle += ", ";
    histTitle += f1DHistoHistogramsDecoder->IthKeyOf(record, iKey);
  }

  int nBins = f1DHistoHistogramsDecoder->NBinsOf(record);

  // this histogram is deallocated when the file is closed
  TH1D* fHistogram = new TH1D(histName.data(), histTitle.data(), nBins, 0, nBins);
 
  for(int iROOTBin = 1; iROOTBin <= nBins; iROOTBin++) {
    fHistogram->SetBinContent(iROOTBin, f1DHistoHistogramsDecoder->IthBinOf(record, iROOTBin-1));
  }

  fHistogram->Write();
  
  return kSuccess;
}

