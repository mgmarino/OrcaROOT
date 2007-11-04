// ORRunDecoder.hh

#ifndef _ORRunDecoder_hh_
#define _ORRunDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORRunDecoder : public ORVBasicTreeDecoder
{
  public:
    ORRunDecoder() {}
    virtual ~ORRunDecoder() {}

    virtual inline bool IsHeartBeat(UInt_t* dataRecord) { return dataRecord[1] & 0x8; }
    virtual inline bool IsRemoteControlRun(UInt_t* dataRecord) { return IsRunStart(dataRecord) && 0x4; }
    virtual inline bool IsQuickStartRun(UInt_t* dataRecord) { return IsRunStart(dataRecord) && 0x2; }
    virtual inline bool IsRunStart(UInt_t* dataRecord) { return !IsHeartBeat(dataRecord) && (dataRecord[1] & 0x1); }
    virtual inline bool IsRunStop(UInt_t* dataRecord) { return !IsHeartBeat(dataRecord) && !(dataRecord[1] & 0x1); }
    virtual inline int RunNumberOf(UInt_t* dataRecord) { return int(dataRecord[2]); }
    virtual inline UInt_t UtimeOf(UInt_t* dataRecord) { return dataRecord[3]; } // for run start/stop
    virtual inline UInt_t TimeToNextHeartBeat(UInt_t* dataRecord) { return UtimeOf(dataRecord); } // for heartbeats

    // for basic trees
    virtual size_t GetNPars() { return 7; }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    virtual std::string GetDataObjectPath() { return "ORRunModel:Run"; }
};

#endif
