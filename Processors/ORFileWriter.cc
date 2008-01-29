// ORFileWriter.cc

#include "ORFileWriter.hh"

#include "TROOT.h"
#include "TObjString.h"
#include "ORLogger.hh"

using namespace std;

ORFileWriter::ORFileWriter(string label)
{
  SetLabel(label);
  fFile = NULL;
}

ORDataProcessor::EReturnCode ORFileWriter::StartRun()
{
  if(fFile != NULL) {
    fFile->Close();
    delete fFile;
  }
  if (!fRunContext) {
    ORLog(kError) << "fRunContext is NULL!" << endl;
    return kFailure;
  }
  string filename = fLabel + ::Form("_run%d.root", fRunContext->GetRunNumber());
  fFile = new TFile(filename.c_str(), "RECREATE");

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
  fFile->Close();
  delete fFile;
  return kSuccess;
}
