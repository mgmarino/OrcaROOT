// ORDataProcessor.hh

#ifndef _ORDataProcessor_hh_
#define _ORDataProcessor_hh_

#include "ORVDataDecoder.hh"

class ORCompoundDataProcessor;
class ORDataProcManager;
class ORRunContext;

class ORDataProcessor
{
  public:
    /**
       Return Codes:
       - kSuccess = normal execution; no warning or error messages should
                    be emitted
       - kFailure = something didn't work, but okay to continue processing;
                    usually accompanied by a warning message, and perhaps an
                    error message; useful for more complicated co-
                    dependencies between elements of a compound analyzer
                    (i.e. if this analyzer failed, do this, otherwise, do
                    that)
       - kBreak   = same as kSuccess/kFailure, but useful for telling a
                    compound analyzer to stop here and move on to the next
                    data record; may be accompanied by a warning message
       - kAlarm   = something is wrong, stop processing data; usually
                    accompanied by an error message
       Note: if StartProcessing() returns kFailure or worse, the analyzer will
       be killed for processing.
    */
    enum EReturnCode { kSuccess, kFailure, kBreak, kAlarm };
    
    ORDataProcessor(ORVDataDecoder* decoder);
    virtual ~ORDataProcessor() {}

    // these are the functions that are typically overloaded
    virtual EReturnCode StartProcessing() { return kSuccess; }
    virtual EReturnCode StartRun() { return kSuccess; }
    virtual EReturnCode ProcessMyDataRecord(UInt_t* /*record*/) { return kSuccess; }
    virtual EReturnCode EndRun() { return kSuccess; }
    virtual EReturnCode EndProcessing() { return kSuccess; }

    virtual UInt_t GetDataId() { return fDataId; } 
    virtual ORVDataDecoder* GetDecoder() { return fDataDecoder; } 

    /*!
     * Gives access to ORRunContext, which allows one to access global
     * parameters associated with a run: run number, sub-run number, etc.
     * This can be useful if a processor needs this information directly. 
     * For example, to access the run number, one could do:
     * GetRunContext()->GetRunNumber();
     * For more information, see the ORRunContext header.
     */
    virtual const ORRunContext* GetRunContext() { return fRunContext; }
    virtual void KillProcessor() { fDoProcess = false; }
    virtual void KillRun() { fDoProcessRun = false; }
    virtual EReturnCode ProcessDataRecord(UInt_t* record);
    virtual void SetDataId();
    virtual void SetDecoderDictionary();
    virtual void SetDoProcess() { fDoProcess = true; fDoProcessRun = true; }
    virtual void SetDoProcessRun() { fDoProcessRun = true; }

    virtual void SetDebugRecord(bool debug = true) { fDebugRecord = debug; }

    /** 
       This is to allow a ORCompoundDataProcessor to access the protected members
       of other ORDataProcessors, for example, SetRunContext, which we want to 
       remain protected. 
     */
    friend class ORCompoundDataProcessor;
    friend class ORDataProcManager;

  protected:
    virtual void SetRunContext(ORRunContext* aContext) { fRunContext = aContext; }
    ORRunContext* fRunContext; 
    UInt_t fDataId;
    bool fDoProcess;
    bool fDoProcessRun;
    ORVDataDecoder* fDataDecoder;
    bool fDebugRecord;
};

#endif
