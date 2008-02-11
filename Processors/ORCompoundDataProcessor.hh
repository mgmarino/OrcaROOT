// ORCompoundDataProcessor.hh

#ifndef _ORCompoundDataProcessor_hh_
#define _ORCompoundDataProcessor_hh_

#include "ORUtilityProcessor.hh"

#include <vector>


class ORCompoundDataProcessor : public ORUtilityProcessor
{
  public:
    ORCompoundDataProcessor();
    virtual ~ORCompoundDataProcessor() {}

    virtual void SetDataId();
    virtual void SetDecoderDictionary();

    virtual EReturnCode StartProcessing(); 
    virtual EReturnCode StartRun();
    virtual EReturnCode ProcessDataRecord(UInt_t* record);
    virtual EReturnCode EndRun();
    virtual EReturnCode EndProcessing();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual void SetDoProcessRun();

    // An ORCompoundDataProcessor running concurrently with other
    // independent analyses under a larger super-compound processor
    // should not return kBreak so that the other compound processors
    // can continue to run. So by default, when a component breaks the
    // processing of an analysis/run/data record, the
    // ORCompoundDataProcessor returns kFailure.  However, for nested 
    // ORCompoundDataProcessors, the break condition may still be desired.
    // the ability to set/change either behavior is provided by the
    // following functions
    virtual void SetComponentBreakReturnsFailure() { fBreakRetCode = kFailure; }
    virtual void SetComponentBreakReturnsBreak() { fBreakRetCode = kBreak; }

    virtual void AddProcessor(ORDataProcessor* processor);
    virtual void ClearProcessors() { fDataProcessors.clear(); }
    virtual void RemoveProcessor(ORDataProcessor* processor);

  protected:
    virtual void SetRunContext(ORRunContext* aContext);
    std::vector<ORDataProcessor*> fDataProcessors;
    EReturnCode fBreakRetCode;
};

#endif
