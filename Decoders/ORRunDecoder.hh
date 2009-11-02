// ORRunDecoder.hh

#ifndef _ORRunDecoder_hh_
#define _ORRunDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORRunDecoder : public ORVBasicTreeDecoder
{
  public:
    ORRunDecoder() {}
    virtual ~ORRunDecoder() {}

    virtual inline Bool_t IsHeartBeat(UInt_t* dataRecord) { return dataRecord[1] & 0x8; }
    virtual inline Bool_t IsRemoteControlRun(UInt_t* dataRecord) { return IsRunStart(dataRecord) && 0x4; }
    virtual inline Bool_t IsQuickStartRun(UInt_t* dataRecord) { return IsRunStart(dataRecord) && 0x2; }

    //! Handles both normal run starts and subruns
    virtual inline Bool_t IsRunStart(UInt_t* dataRecord) 
      { return !IsHeartBeat(dataRecord) && (dataRecord[1] & 0x1); }
    virtual inline Bool_t IsRunStop(UInt_t* dataRecord) { return !IsHeartBeat(dataRecord) && !(dataRecord[1] & 0x1); }
    virtual inline Bool_t IsPrepareForSubRun(UInt_t* dataRecord) { return !IsHeartBeat(dataRecord) && (dataRecord[1] & 0x10); }
    virtual inline Bool_t IsSubRunStart(UInt_t* dataRecord) 
      { return !IsHeartBeat(dataRecord) && (dataRecord[1] & 0x20); }

    virtual inline Int_t RunNumberOf(UInt_t* dataRecord) { return Int_t(dataRecord[2]); }
    virtual inline Int_t SubRunNumberOf(UInt_t* dataRecord) { return Int_t(dataRecord[1] >> 16); }
    virtual inline UInt_t UtimeOf(UInt_t* dataRecord) { return dataRecord[3]; } // for run start/stop
    virtual inline UInt_t TimeToNextHeartBeat(UInt_t* dataRecord) { return UtimeOf(dataRecord); } // for heartbeats

    // for basic trees
    virtual size_t GetNPars() { return 9; }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    virtual std::string GetDataObjectPath() { return "ORRunModel:Run"; }
};

#endif
