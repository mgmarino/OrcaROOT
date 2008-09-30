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
    virtual EReturnCode EndProcessing();

    virtual std::string GetLabel() { return fLabel; }
    virtual void SetLabel(std::string label) { fLabel = label; }

  protected:
    virtual TFile* UpdateFilePointer();

  protected:
    std::string fLabel;
    std::string fSavedName;
    TFile* fFile;
};

#endif
