// ORRunContext.hh

#ifndef _ORRunContext_hh_
#define _ORRunContext_hh_

#include <string>
#include "ORHeader.hh"
#include "ORHardwareDictionary.hh"


class ORRunDataProcessor;
class ORDataProcManager;
class ORVWriter;

class ORRunContext
{

  friend class ORRunDataProcessor;
  friend class ORDataProcManager;
  /* These classes are managers and so they have access to the protected 
   * members of ORRunContext.  This is to improve data hiding. */
  public:
    enum EState { kIdle, kStarting, kRunning, kStopping };

    ORRunContext(ORHeader* header = NULL, const char* runCtrlPath = NULL);
    virtual ~ORRunContext();

    virtual std::string GetClassName() { return fClassName; }
    virtual int GetRunNumber() const { return fRunNumber; }
    virtual bool IsQuickStartRun() const { return fIsQuickStartRun; }
    virtual int GetRunType() const { return fRunType; }
    virtual int GetStartTime() const { return fStartTime; }
    virtual const ORHeader* GetHeader() const { return fHeader; }
    virtual const ORHardwareDictionary* GetHardwareDict() const { return fHardwareDict; }
    virtual inline bool MustSwap() const { return fMustSwap; }
    virtual inline bool IsRecordSwapped() const { return fIsRecordSwapped; }

    virtual int* GetPointerToRunNumber() { return &fRunNumber; }
    virtual inline EState GetState() { return fState; }
    virtual Int_t WriteBackToSocket(const void* buffer, size_t nBytes);

    // for use by a managing process
    // FIXME: allow only ORRunDataProcessor to call this?
    virtual void SetRecordSwapped() { fIsRecordSwapped = true; }
    

  protected:
    virtual void ResetRecordFlags() { fIsRecordSwapped = false; }
    virtual bool LoadHeader(ORHeader* header, 
      bool ignoreRunControl = false,
      const char* runCtrlPath = "ObjectInfo:DataChain");
      /* Returns true if success, false if not. */
    virtual void LoadRunStartRecord(UInt_t* record);
    virtual void SetWritableSocket(ORVWriter* aSocket) { fWritableSocket = aSocket; }

    /* To be called at the beginning of each record by a managing processor. */
    virtual void SetIdle();
    virtual void SetStarting();
    virtual void SetRunning();
    virtual void SetStopping();
    virtual void SetMustSwap(bool mustSwap) { fMustSwap = mustSwap; } 

    ORHeader* fHeader;
    ORHardwareDictionary* fHardwareDict;
    std::string fClassName;
    int fRunNumber;
    bool fIsQuickStartRun;
    bool fIsRecordSwapped;
    bool fMustSwap;
    int fRunType;
    int fStartTime;

    ORVWriter* fWritableSocket;

    EState fState;
};

#endif
