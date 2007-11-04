// ORRunContext.hh

#ifndef _ORRunContext_hh_
#define _ORRunContext_hh_

#include <string>
#include "ORHeader.hh"

class ORRunContext
{
  public:
    enum EState { kIdle, kStarting, kRunning, kStopping };

    ORRunContext(ORHeader* header = NULL, const char* runCtrlPath = NULL);
    virtual ~ORRunContext() {}

    virtual std::string GetClassName() { return fClassName; }
    virtual int GetRunNumber() const { return fRunNumber; }
    virtual bool IsQuickStartRun() const { return fIsQuickStartRun; }
    virtual int GetRunType() const { return fRunType; }
    virtual int GetStartTime() const { return fStartTime; }
    virtual void LoadHeader(ORHeader* header, const char* runCtrlPath = "Run Control");
    virtual void LoadRunStartRecord(UInt_t* record);
    virtual ORHeader* GetHeader() { return fHeader; }

    virtual int* GetPointerToRunNumber() { return &fRunNumber; }

    // for use by a managing process
    // FIXME: allow only ORRunDataProcessor to call these?
    virtual inline bool IsRecordSwapped() {return fIsRecordSwapped;}
    virtual void SetRecordSwapped(bool swap = true) {fIsRecordSwapped = swap;}
    virtual void ResetRecordFlags() {SetRecordSwapped(false);}
    /* To be called at the beginning of each record by a managing processor. */
    virtual inline EState GetState() { return fState; }
    virtual void SetIdle();
    virtual void SetStarting();
    virtual void SetRunning();
    virtual void SetStopping();

  protected:
    ORHeader* fHeader;
    std::string fClassName;
    int fRunNumber;
    bool fIsQuickStartRun;
    bool fIsRecordSwapped;
    int fRunType;
    int fStartTime;

    EState fState;
};

#endif
