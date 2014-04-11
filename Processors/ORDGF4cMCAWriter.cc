// ORDGF4cMCAWriter.cc

#include "ORDGF4cMCAWriter.hh"
#include "ORLogger.hh"
#include "TH1.h"
#include "TROOT.h"

using namespace std;

ORDGF4cMCAWriter::ORDGF4cMCAWriter(string histogramName) :
ORDataProcessor(new ORDGF4cMCADecoder)
{
  fMCADecoder = dynamic_cast<ORDGF4cMCADecoder*>(fDataDecoder);
  fHistNameBase = histogramName;
}

ORDataProcessor::EReturnCode ORDGF4cMCAWriter::ProcessMyDataRecord(UInt_t* record)
{
  string histName = fHistNameBase;
  int iHist=1;
  while(gDirectory->Get(histName.data()) != NULL) {
    histName = Form("%s%02d", fHistNameBase.data(), iHist);
    iHist++;
  }

  string histTitle = "MCA Spectrum";

  int nBins = 32768;

  // this histogram is deallocated when the file is closed
  TH1D* fHistogram = new TH1D(histName.data(), histTitle.data(), nBins, 0, nBins);
 
  for(int iROOTBin = 1; iROOTBin <= nBins; iROOTBin++) {
    fHistogram->SetBinContent(iROOTBin, fMCADecoder->IthBinOf(record, iROOTBin-1));
  }

  fHistogram->Write();
  
  return kSuccess;
}

