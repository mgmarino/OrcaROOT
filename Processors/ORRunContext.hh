// ORRunContext.hh

#ifndef _ORRunContext_hh_
#define _ORRunContext_hh_

#include <string>
#include "ORHeader.hh"
#include "ORHardwareDictionary.hh"


class ORRunDataProcessor;
class ORDataProcManager;
class ORVWriter;


/*!
 *  ORRunContext provides an object for encapsulating the
 *  information surrounding an ORCA data run.  It provides an
 *  access point for the following information:
 *  Run Number - number of the run
 *  Run State - Particular state of the run (see ORRunContext::EState)
 *  Sub Run Number - number of the sub run
 *  Header - provides direct access to the header information via ORHeader
 *  Hardware Dictionary - provides access to the hardware dictionary,
 *    a dictionary encapsulating information on cards/crates and keyed
 *    according to crate, card number.
 *  Run start time - actual start time of the run
 *  Run Type - ORCA run type
 *  Packet Number - How many packets have been read during run. Useful for
 *    indexing packets.

 *  ORRunContext handles several other 'global' aspects of a run, including
 *  determining if the current record is already swapped, and providing
 *  pointer access to a writable socket to return data across a socket.
 *  This latter functionality is used by ORVOrcaRequestProcessor to 
 *  handle communication back to a running ORCA program.
 *  
 *  In general, a global ORRunContext is held by ORDataProcManager
 *  which then passes the context onto the other processors.  Thus,
 *  all ORDataProcessors can access the ORRunContext via the member
 *  function ORDataProcessor::GetRunContext()
 */
class ORRunContext
{

  friend class ORRunDataProcessor;
  friend class ORDataProcManager;
  /* These classes are managers and so they have access to the protected 
   * members of ORRunContext.  This is to improve data hiding. */
  public:
    /*!
     * A list of available states that the ORCA file can be 
     * in during processing.  
     * The following states are possible:
     *   Idle
     *   Starting
     *   Running
     *   PreparingForSubRun
     *   Stopping
     */

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
    virtual Int_t GetStopTime() const { return fStopTime; }
    virtual Int_t GetPacketNumber() const { return fPacketNumber; }
    virtual const ORHeader* GetHeader() const { return fHeader; }
    virtual const ORHardwareDictionary* GetHardwareDict() const { return fHardwareDict; }
    virtual inline Bool_t MustSwap() const { return fMustSwap; }
    virtual inline Bool_t IsRecordSwapped() const { return fIsRecordSwapped; }

    //! Pointer access function for Run number
    /*!
        This is useful when needed to, for example, initialize
        the branch of a TTree with the pointer of this variable
     */
    virtual Int_t* GetPointerToRunNumber() { return &fRunNumber; }
    //! Pointer access function for Sub Run number
    /*!
        This is useful when needed to, for example, initialize
        the branch of a TTree with the pointer of this variable
     */
    virtual Int_t* GetPointerToSubRunNumber() { return &fSubRunNumber; }
    //! Pointer access function for string of state 
    /*!
        This is useful when needed to, for example, initialize
        the branch of a TTree with the pointer of this variable
     */
    virtual std::string* GetPointerToStringOfState() { return &fStringOfState; }

    virtual inline EState GetState() const { return fState; }
    virtual void SetState(EState aState) 
      { fState = aState; fStringOfState = GetStateName(fState); }

    /*!
     * The following (string) states are possible:
     *   Idle
     *   Starting
     *   Running
     *   PreparingForSubRun
     *   Stopping
     */
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
    virtual void LoadRunStopRecord(UInt_t* record);
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
    Int_t fStopTime; // won't be valid until the end of a run
    std::string fStringOfState; 
    Int_t fPacketNumber;

    ORVWriter* fWritableSocket;

    EState fState;
};

#endif
