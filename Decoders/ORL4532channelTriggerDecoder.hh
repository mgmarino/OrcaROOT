// ORL4532channelTriggerDecoder.hh

#ifndef _ORL4532channelTriggerDecoder_hh_
#define _ORL4532channelTriggerDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORL4532channelTriggerDecoder : public ORVBasicTreeDecoder
{
  public:
    ORL4532channelTriggerDecoder() {}
    virtual ~ORL4532channelTriggerDecoder() {}

    virtual inline UInt_t CrateOf(UInt_t* record)
      { return (record[1] & 0x01e00000) >> 21; }
    virtual inline UInt_t CardOf(UInt_t* record)
      { return (record[1] & 0x001f0000) >> 16; }
    virtual inline UInt_t TriggerMaskOf(UInt_t* record) { return record[2]; }

    virtual std::string GetDataObjectPath() { return "ORL4532Model:channelTrigger"; }

    // for basic trees
    virtual size_t GetNPars() { return 3; }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);
};

#endif
