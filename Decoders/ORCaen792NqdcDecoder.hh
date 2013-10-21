// ORCaen792NqdcDecoder.hh

#ifndef _ORCaen792NqdcDecoder_hh_
#define _ORCaen792NqdcDecoder_hh_

#include "ORCaen792qdcDecoder.hh"

class ORCaen792NqdcDecoder : public ORCaen792qdcDecoder
{
  public:
    virtual inline UInt_t IthChannelOf(UInt_t* record, size_t iRow)
      { return (GetLocPtr(record, iRow)[0] & 0x003e0000) >> 17; }
    virtual std::string GetDataObjectPath() { return "ORCaen792Model:QdcN"; }
    virtual size_t GetNChannels() { return 16; }
};

#endif
