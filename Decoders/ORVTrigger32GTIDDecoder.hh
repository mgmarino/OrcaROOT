// ORVTrigger32GTIDDecoder.hh

#ifndef _ORVTrigger32GTIDDecoder_hh_
#define _ORVTrigger32GTIDDecoder_hh_

#include "ORVBasicTreeDecoder.hh"
#include "ORVEventCounterDecoder.hh"

class ORVTrigger32GTIDDecoder : public ORVBasicTreeDecoder, public ORVEventCounterDecoder
{
  public:
    ORVTrigger32GTIDDecoder() {}
    virtual ~ORVTrigger32GTIDDecoder() {}

    virtual inline UInt_t EventTriggerBitsOf(UInt_t* record)
      { return IsShort(record) ? (record[0] & 0x03000000) >> 24 : (record[1] & 0x03000000) >> 24; }
    virtual inline UInt_t GTIDOf(UInt_t* record)
      { return IsShort(record) ? (record[0] & 0x000fffff) : (record[1] & 0x000fffff); }

    // for basic trees
    virtual inline size_t GetNPars() { return 2; }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    // for event counters
    virtual inline size_t GetEventCount(UInt_t* /*record*/) { return 1; }
};

class ORTrigger32GTID1Decoder : public ORVTrigger32GTIDDecoder
{
  public:
    virtual ~ORTrigger32GTID1Decoder() {}
    virtual inline std::string GetDataObjectPath() { return "ORTrigger32Model:GTID1 Record"; }
};

class ORTrigger32GTID2Decoder : public ORVTrigger32GTIDDecoder
{
  public:
    virtual ~ORTrigger32GTID2Decoder() {}
    virtual inline std::string GetDataObjectPath() { return "ORTrigger32Model:GTID2 Record"; }
};
#endif
