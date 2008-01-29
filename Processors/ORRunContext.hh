// ORRunContext.hh

#ifndef _ORRunContext_hh_
#define _ORRunContext_hh_

#include <string>
#include "ORHeader.hh"
#include "ORHardwareDictionary.hh"

class ORRunContext
{
  public:
    enum EState { kIdle, kStarting, kRunning, kStopping };

    ORRunContext(ORHeader* header = NULL, const char* runCtrlPath = NULL);
    virtual ~ORRunContext();

    virtual std::string GetClassName() { return fClassName; }
    virtual int GetRunNumber() const { return fRunNumber; }
    virtual bool IsQuickStartRun() const { return fIsQuickStartRun; }
    virtual int GetRunType() const { return fRunType; }
    virtual int GetStartTime() const { return fStartTime; }
    virtual bool LoadHeader(ORHeader* header, const char* runCtrlPath = "ObjectInfo:DataChain");
      /* Returns true if success, false if not. */
    virtual void LoadRunStartRecord(UInt_t* record);
    virtual ORHeader* GetHeader() { return fHeader; }
    virtual ORHardwareDictionary* GetHardwareDict() { return fHardwareDict; }

    virtual int* GetPointerToRunNumber() { return &fRunNumber; }

    // for use by a managing process
    // FIXME: allow only ORRunDataProcessor to call these?
    virtual inline bool MustSwap() const { return fMustSwap; }
    virtual inline bool IsRecordSwapped() const { return fIsRecordSwapped; }
    virtual void SetRecordSwapped(bool swap = true) { fIsRecordSwapped = swap; }
    virtual void ResetRecordFlags() { SetRecordSwapped(false); }
    /* To be called at the beginning of each record by a managing processor. */
    virtual inline EState GetState() { return fState; }
    virtual void SetIdle();
    virtual void SetStarting();
    virtual void SetRunning();
    virtual void SetStopping();
    virtual void SetMustSwap(bool mustSwap) { fMustSwap = mustSwap; } 

  protected:
    ORHeader* fHeader;
    ORHardwareDictionary* fHardwareDict;
    std::string fClassName;
    int fRunNumber;
    bool fIsQuickStartRun;
    bool fIsRecordSwapped;
    bool fMustSwap;
    int fRunType;
    int fStartTime;

    EState fState;
};

#endif
