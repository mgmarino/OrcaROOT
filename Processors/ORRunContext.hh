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
    enum EState { kIdle=0, 
                  kStarting, 
                  kRunning, 
                  kPreparingForSubRun, 
                  kStopping, 
                  kNumberOfStates};

    ORRunContext(ORHeader* header = NULL, const char* runCtrlPath = NULL);
    virtual ~ORRunContext();

    virtual std::string GetClassName() const { return fClassName; }
    virtual Int_t GetRunNumber() const { return fRunNumber; }
    virtual Int_t GetSubRunNumber() const { return fSubRunNumber; }
    virtual Bool_t IsQuickStartRun() const { return fIsQuickStartRun; }
    virtual Int_t GetRunType() const { return fRunType; }
    virtual Int_t GetStartTime() const { return fStartTime; }
    virtual const ORHeader* GetHeader() const { return fHeader; }
    virtual const ORHardwareDictionary* GetHardwareDict() const { return fHardwareDict; }
    virtual inline Bool_t MustSwap() const { return fMustSwap; }
    virtual inline Bool_t IsRecordSwapped() const { return fIsRecordSwapped; }

    virtual Int_t* GetPointerToRunNumber() { return &fRunNumber; }
    virtual Int_t* GetPointerToSubRunNumber() { return &fSubRunNumber; }
    virtual std::string* GetPointerToStringOfState() { return &fStringOfState; }
    virtual inline EState GetState() const { return fState; }
    virtual void SetState(EState aState) 
      { fState = aState; fStringOfState = GetStateName(fState); }
    virtual std::string GetStateName(EState aState) const;
    virtual inline Int_t GetNumberOfStates() const {return (Int_t)kNumberOfStates;}


    virtual Int_t WriteBackToSocket(const void* buffer, size_t nBytes);

    // for use by a managing process
    // FIXME: allow only ORRunDataProcessor to call this?
    virtual void SetRecordSwapped() { fIsRecordSwapped = true; }
    

  protected:
    virtual void ResetRecordFlags() { fIsRecordSwapped = false; }
    virtual Bool_t LoadHeader(ORHeader* header, 
      Bool_t ignoreRunControl = false,
      const char* runCtrlPath = "ObjectInfo:DataChain");
      /* Returns true if success, false if not. */
    virtual void LoadRunStartRecord(UInt_t* record);
    virtual void SetWritableSocket(ORVWriter* aSocket) { fWritableSocket = aSocket; }

    /* To be called at the beginning of each record by a managing processor. */
    virtual void SetIdle();
    virtual void SetStarting();
    virtual void SetRunning();
    virtual void SetStopping();
    virtual void SetPreparingForSubRun();
    virtual void SetMustSwap(Bool_t mustSwap) { fMustSwap = mustSwap; } 

    ORHeader* fHeader;
    ORHardwareDictionary* fHardwareDict;
    std::string fClassName;
    Int_t fRunNumber;
    Int_t fSubRunNumber;
    Bool_t fIsQuickStartRun;
    Bool_t fIsRecordSwapped;
    Bool_t fMustSwap;
    Int_t fRunType;
    Int_t fStartTime;
    std::string fStringOfState; 

    ORVWriter* fWritableSocket;

    EState fState;
};

#endif
