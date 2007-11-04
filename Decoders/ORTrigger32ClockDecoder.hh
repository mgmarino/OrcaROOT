// ORTrigger32ClockDecoder.hh

#ifndef _ORTrigger32ClockDecoder_hh_
#define _ORTrigger32ClockDecoder_hh_

#include "ORVBasicTreeDecoder.hh"
#include "ORVEventCounterDecoder.hh"

class ORTrigger32ClockDecoder : public ORVBasicTreeDecoder, public ORVEventCounterDecoder
{
  public:
    ORTrigger32ClockDecoder() {}
    virtual ~ORTrigger32ClockDecoder() {}

    virtual inline UInt_t EventTriggerBitsOf(UInt_t* record)
      { return (record[1] & 0x003000000) >> 24; }
    virtual inline UInt_t MacTimeTopBitsOf(UInt_t* record)
      { return record[1] & 0x000ffffff; }
    virtual inline UInt_t MacTimeBottomBitsOf(UInt_t* record)
      { return record[2]; }
    virtual inline ULong64_t MacTimeOf(UInt_t* record)
      { return (((ULong64_t) MacTimeTopBitsOf(record)) << 32) + MacTimeBottomBitsOf(record); }
    virtual inline double GetMacTime_s(UInt_t* record)
      { return double(MacTimeOf(record)) * 1.0e-7; }

    // for basic trees
    virtual inline size_t GetNPars() { return 3; }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    // for event counters
    virtual inline size_t GetEventCount(UInt_t* /*record*/) { return 1; }

    virtual inline std::string GetDataObjectPath() { return "ORTrigger32Model:100MHz Clock Record"; }
};

#endif
