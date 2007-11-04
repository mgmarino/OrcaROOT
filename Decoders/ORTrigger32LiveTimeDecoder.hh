// ORTrigger32LiveTimeDecoder.hh

#ifndef _ORTrigger32LiveTimeDecoder_hh_
#define _ORTrigger32LiveTimeDecoder_hh_

#include "ORVBasicTreeDecoder.hh"
#include "ORVEventCounterDecoder.hh"

class ORTrigger32LiveTimeDecoder : public ORVBasicTreeDecoder, public ORVEventCounterDecoder
{
  public:
    enum ERunState { kStopRun = 0, kStartRun = 1, kMiddleRun = 3 };
    ORTrigger32LiveTimeDecoder() {}
    virtual ~ORTrigger32LiveTimeDecoder() {}

    virtual inline UInt_t EventTriggerBitsOf(UInt_t* record)
      { return (record[1] & 0x0c000000) >> 24; }
    virtual inline UInt_t GTIDOf(UInt_t* record)
      { return (record[1] & 0x00ffffff); }
    virtual inline ERunState RunStateOf(UInt_t* record)
      { return (ERunState) ((record[2] & 0x00030000) >> 16); }
    virtual inline UInt_t CrateOf(UInt_t* record)
      { return (record[2] & 0x00000f00) >> 8; }
    virtual inline UInt_t SlotOf(UInt_t* record)
      { return record[2] & 0x0000001f; }

    virtual inline UInt_t TotalLiveTimeTopOf(UInt_t* record)
      { return record[3] & 0x000000ff; }
    virtual inline UInt_t TotalLiveTimeBottomOf(UInt_t* record)
      { return record[4]; }
    virtual inline ULong64_t TotalLiveTimeOf(UInt_t* record) 
      { return (((ULong64_t) TotalLiveTimeTopOf(record)) << 32) + TotalLiveTimeBottomOf(record); }
    virtual inline double GetTotalLiveTime_s(UInt_t* record) 
      { return double(TotalLiveTimeOf(record))*1.0e-6; }

    virtual inline UInt_t Trigger1LiveTimeTopOf(UInt_t* record)
      { return (record[3] & 0x0000ff00) >> 8; }
    virtual inline UInt_t Trigger1LiveTimeBottomOf(UInt_t* record)
      { return record[5]; }
    virtual inline ULong64_t Trigger1LiveTimeOf(UInt_t* record) 
      { return (((ULong64_t) Trigger1LiveTimeTopOf(record)) << 32) + Trigger1LiveTimeBottomOf(record); }
    virtual inline double GetTrigger1LiveTime_s(UInt_t* record) 
      { return double(Trigger1LiveTimeOf(record))*1.0e-6; }

    virtual inline UInt_t Trigger2LiveTimeTopOf(UInt_t* record)
      { return (record[3] & 0x00ff0000) >> 16; }
    virtual inline UInt_t Trigger2LiveTimeBottomOf(UInt_t* record)
      { return record[6]; }
    virtual inline ULong64_t Trigger2LiveTimeOf(UInt_t* record) 
      { return (((ULong64_t) Trigger2LiveTimeTopOf(record)) << 32) + Trigger2LiveTimeBottomOf(record); }
    virtual inline double GetTrigger2LiveTime_s(UInt_t* record) 
      { return double(Trigger2LiveTimeOf(record))*1.0e-6; }

    virtual inline UInt_t ScopeLiveTimeTopOf(UInt_t* record)
      { return (record[3] & 0xff000000) >> 24; }
    virtual inline UInt_t ScopeLiveTimeBottomOf(UInt_t* record)
      { return record[7]; }
    virtual inline ULong64_t ScopeLiveTimeOf(UInt_t* record) 
      { return (((ULong64_t) ScopeLiveTimeTopOf(record)) << 32) + ScopeLiveTimeBottomOf(record); }
    virtual inline double GetScopeLiveTime_s(UInt_t* record) 
      { return double(ScopeLiveTimeOf(record))*1.0e-6; }

    // for basic trees
    virtual inline size_t GetNPars() { return 13; }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    // for event counters
    virtual inline size_t GetEventCount(UInt_t* /*record*/) { return 1; }

    virtual inline std::string GetDataObjectPath() { return "ORTrigger32Model:LiveTime"; }
};

#endif
