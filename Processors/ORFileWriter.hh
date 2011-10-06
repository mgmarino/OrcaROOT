// ORFileWriter.hh

#ifndef _ORFileWriter_hh_
#define _ORFileWriter_hh_

#include <string>
#include "TFile.h"
#include "ORUtilityProcessor.hh"


class ORFileWriter : public ORUtilityProcessor
{
  public:
    ORFileWriter(std::string label = "OR");
    virtual ~ORFileWriter() {}

    virtual EReturnCode StartRun();
    virtual EReturnCode EndRun();

    // The following function is used to check if we are in a new sub-run. Since
    // the original OrcaRoot architects decided to write the header to file in
    // this class, we will also write the subrun headers from here.
    virtual EReturnCode ProcessDataRecord(UInt_t*);

    virtual EReturnCode EndProcessing();

    virtual std::string GetLabel() { return fLabel; }
    virtual void SetLabel(std::string label) { fLabel = label; }

  protected:
    virtual TFile* UpdateFilePointer();
  
  protected:
    std::string fLabel;
    std::string fSavedName;
    TFile* fFile;
    Int_t fLastSubRunNumber;
};

#endif
