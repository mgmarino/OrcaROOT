// ORDataProcManager.hh

#ifndef _ORDataProcManager_hh_
#define _ORDataProcManager_hh_

#include <string>
#include <vector>
#include "ORVReader.hh"
#include "ORCompoundDataProcessor.hh"
#include "ORHeaderProcessor.hh"
#include "ORRunDataProcessor.hh"
#include "ORVSigHandler.hh"

class ORDataProcManager : public ORCompoundDataProcessor, public ORVSigHandler
{
  public:
    ORDataProcManager(ORVReader* reader = NULL, ORRunDataProcessor* runDataProc = NULL);
    virtual ~ORDataProcManager();

    virtual EReturnCode ProcessDataStream();
    virtual EReturnCode ProcessRun();
    virtual void SetReader(ORVReader* reader) { fReader = reader; }
    virtual void SetDataId();

    virtual void Handle(int signal);

  protected:
    virtual void SetRunContext(ORRunContext* aContext);
    ORVReader* fReader;
    ORHeaderProcessor fHeaderProcessor;
    ORRunDataProcessor* fRunDataProcessor;
    bool fIOwnRunDataProcessor;
};

#endif
