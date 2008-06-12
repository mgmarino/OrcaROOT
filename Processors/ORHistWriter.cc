// ORHistWriter.cc

#include "ORHistWriter.hh"

#include "TH2.h"
#include "TH3.h"
#include "ORLogger.hh"

using namespace std;

ORHistWriter::ORHistWriter(ORVHistDecoder* histDecoder) :
ORDataProcessor(histDecoder)
{
  fHistDecoder = histDecoder;
}

ORDataProcessor::EReturnCode ORHistWriter::StartProcessing()
{
  if(fHistDecoder == NULL) {
    ORLog(kWarning) << "StartProcessing(): fHistDecoder was NULL, can't proceed";
    KillProcessor();
    return kFailure;
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORHistWriter::StartRun()
{
  map<int, TH1*>::iterator iHist;
  for (iHist = fHists.begin(); iHist != fHists.end(); iHist++) {
    if (iHist->second != NULL) iHist->second->Reset();
  }
 
  return kSuccess;
}

ORDataProcessor::EReturnCode ORHistWriter::ProcessMyDataRecord(UInt_t* record)
{
  int iHist = fHistDecoder->GetHistIndex(record);

  // look up in map only once for a typical record
  TH1* hist = fHists[iHist]; 

  if(hist == NULL) {
    switch(fHistDecoder->GetNDim()) {
      // fHists are owned by parent root file (or gROOT); 
      // they will be deleted upon fFile->Close() (or end of program)
      case 1:
        fHists[iHist] = new TH1D(
	  fHistDecoder->GetHistName(iHist).c_str(), fHistDecoder->GetHistTitle(iHist).c_str(),
	  fHistDecoder->GetNbinsX(), fHistDecoder->GetXLo(), fHistDecoder->GetXHi()
	);
	fHists[iHist]->SetXTitle(fHistDecoder->GetXTitle().c_str());
        break;
      case 2:
        fHists[iHist] = new TH2D(
	  fHistDecoder->GetHistName(iHist).c_str(), fHistDecoder->GetHistTitle(iHist).c_str(),
	  fHistDecoder->GetNbinsX(), fHistDecoder->GetXLo(), fHistDecoder->GetXHi(),
	  fHistDecoder->GetNbinsY(), fHistDecoder->GetYLo(), fHistDecoder->GetYHi()
	);
	fHists[iHist]->SetXTitle(fHistDecoder->GetXTitle().c_str());
	fHists[iHist]->SetYTitle(fHistDecoder->GetYTitle().c_str());
        break;
      case 3:
        fHists[iHist] = new TH3D(
	  fHistDecoder->GetHistName(iHist).c_str(), fHistDecoder->GetHistTitle(iHist).c_str(),
	  fHistDecoder->GetNbinsX(), fHistDecoder->GetXLo(), fHistDecoder->GetXHi(),
	  fHistDecoder->GetNbinsY(), fHistDecoder->GetYLo(), fHistDecoder->GetYHi(),
	  fHistDecoder->GetNbinsZ(), fHistDecoder->GetZLo(), fHistDecoder->GetZHi()
	);
	fHists[iHist]->SetXTitle(fHistDecoder->GetXTitle().c_str());
	fHists[iHist]->SetYTitle(fHistDecoder->GetYTitle().c_str());
	fHists[iHist]->SetZTitle(fHistDecoder->GetZTitle().c_str());
        break;
      default:
        ORLog(kWarning) << "ProcessMyDataRecord(): Can't handle more than "
	                << "3 dimensions; using 1..." << endl;
        fHists[iHist] = new TH1D(
	  fHistDecoder->GetHistName(iHist).c_str(), fHistDecoder->GetHistTitle(iHist).c_str(),
	  fHistDecoder->GetNbinsX(), fHistDecoder->GetXLo(), fHistDecoder->GetXHi()
	);
    }
    hist = fHists[iHist];
  }

  for(size_t iEntry=0; iEntry < fHistDecoder->GetNEntries(record); iEntry++) {
    switch(fHistDecoder->GetNDim()) {
      case 1: 
        hist->Fill(fHistDecoder->GetX(record, iEntry), 
                   fHistDecoder->GetWeight(record, iEntry));
        break;
      case 2: 
        ((TH2D*) hist)->Fill(fHistDecoder->GetX(record, iEntry), 
                             fHistDecoder->GetY(record, iEntry), 
                             fHistDecoder->GetWeight(record, iEntry));
        break;
      case 3: 
        ((TH3D*) hist)->Fill(fHistDecoder->GetX(record, iEntry), 
                             fHistDecoder->GetY(record, iEntry), 
                             fHistDecoder->GetZ(record, iEntry),
                             fHistDecoder->GetWeight(record, iEntry));
        break;
      default:
        hist->Fill(fHistDecoder->GetX(record, iEntry), 
                   fHistDecoder->GetWeight(record, iEntry));
        break;
    }
  }

  return kSuccess;
}

ORDataProcessor::EReturnCode ORHistWriter::EndRun()
{
  map<int, TH1*>::iterator iHist;
  for (iHist = fHists.begin(); iHist != fHists.end(); iHist++) {
    if (iHist->second != NULL) {
      iHist->second->Write();
      iHist->second->Reset();
    }
  }

  // if a TFile is open, the histos will be deleted by the file. If not,
  // gDirectory will be named "Rint", and the histos will remain in memory.
  if(string(gDirectory->GetName()) != "Rint") {
    fHists.clear();
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORHistWriter::EndProcessing()
{
  if(string(gDirectory->GetName()) != "Rint") return kSuccess;

  map<int, TH1*>::iterator iHist;
  for (iHist = fHists.begin(); iHist != fHists.end(); iHist++) {
    delete iHist->second;
  }
  return kSuccess;
}
