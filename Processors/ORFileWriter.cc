// ORFileWriter.cc

#include "ORFileWriter.hh"

#include "TROOT.h"
#include "TObjString.h"
#include "ORLogger.hh"
#include "ORRunContext.hh"

using namespace std;

ORFileWriter::ORFileWriter(string label)
{
  fLabel = label;
  fSavedName = "";
  fFile = NULL;
}

ORDataProcessor::EReturnCode ORFileWriter::StartRun()
{
  if(fFile != NULL) {
    if(UpdateFilePointer() == NULL) {
      ORLog(kError) << "Lost track of fFile!" << endl;
      return kFailure;
    }
    fFile->Close();
    delete fFile;
  }
  if (!fRunContext) {
    ORLog(kError) << "fRunContext is NULL!" << endl;
    return kFailure;
  }
  string filename = fLabel + ::Form("_run%d.root", fRunContext->GetRunNumber());
  fFile = new TFile(filename.c_str(), "RECREATE");
  fSavedName = fFile->GetName();
  fSavedName.erase( fSavedName.size() - 5, 5 ); // Removing .root from the end

  TObjString headerXML(fRunContext->GetHeader()->GetRawXML().Data());
  headerXML.Write("headerXML");

  return kSuccess;
}

ORDataProcessor::EReturnCode ORFileWriter::EndRun()
{
  if(fFile == NULL) {
    ORLog(kWarning) << "EndRun(): ending a run that never began!" << endl;
    return kBreak;
  }

  if(UpdateFilePointer() == NULL) {
    ORLog(kError) << "Lost track of fFile!" << endl;
    return kFailure;
  }
  fFile->cd();
  // other processors will write their data after this processor; close the file at
  // the beginning of the next run or at the end of processing.
  return kSuccess;
}

ORDataProcessor::EReturnCode ORFileWriter::EndProcessing()
{
  if(fFile == NULL) {
    ORLog(kWarning) << "EndProcessing(): no file was open!" << endl;
    return kBreak;
  } 
  if(UpdateFilePointer() == NULL) {
    ORLog(kError) << "Lost track of fFile!" << endl;
    return kFailure;
  }
  fFile->Close();
  delete fFile;
  return kSuccess;
}

TFile* ORFileWriter::UpdateFilePointer()
{
  TSeqCollection* listOfFiles = gROOT->GetListOfFiles();
  if(listOfFiles->IndexOf(fFile) == -1) {
    // fFile has been deleted by stupid ROOT. Search for the updated pointer
    fFile = NULL;
    for(int i=0; fFile == NULL && i<=listOfFiles->LastIndex(); i++) {
      TFile* file = (TFile*) listOfFiles->At(i);
      std::string nameOfFile = file->GetName();
      if ( nameOfFile.find( fSavedName ) != string::npos ) {
        fFile = file;
        break;
      }
    }
  }
  // If the updated pointer is not found, fFile is NULL here. Could be a
  // bug, let the calling function decide.
  return fFile;
}

