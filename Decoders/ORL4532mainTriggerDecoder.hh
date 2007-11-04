// ORL4532mainTriggerDecoder.hh

#ifndef _ORL4532mainTriggerDecoder_hh_
#define _ORL4532mainTriggerDecoder_hh_

#include "ORVBasicRDTreeDecoder.hh"

class ORL4532mainTriggerDecoder : public ORVBasicRDTreeDecoder
{
  public:
    ORL4532mainTriggerDecoder() {}
    virtual ~ORL4532mainTriggerDecoder() {}

    virtual inline UInt_t EventCountOf(UInt_t* record) { return record[1]; }
    virtual inline bool HasDoubleWordTimestamp(UInt_t* record)
      { return (record[2] & 0x02000000) >> 25; }
    virtual inline UInt_t CrateOf(UInt_t* record)
      { return (record[2] & 0x01e00000) >> 21; }
    virtual inline UInt_t CardOf(UInt_t* record)
      { return (record[2] & 0x001f0000) >> 16; }
    virtual double ReferenceDateOf(UInt_t* record);

    virtual std::string GetDataObjectPath() { return "ORL4532Model:mainTrigger"; }

    // for basic trees
    virtual size_t GetNPars() { return 3; }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);
};

#endif
