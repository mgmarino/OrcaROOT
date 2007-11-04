// ORTek754DScopeDataDecoder.hh

#ifndef _ORTek754DScopeDataDecoder_hh_
#define _ORTek754DScopeDataDecoder_hh_

#include "ORVEventCounterDecoder.hh"

class ORTek754DScopeDataDecoder : public ORVEventCounterDecoder
{
  public:
    ORTek754DScopeDataDecoder() {}
    virtual ~ORTek754DScopeDataDecoder() {}

    virtual inline UInt_t GPIBAddressOf(UInt_t* record)
      { return (record[1] & 0x07800000) >> 23; }
    virtual inline UInt_t ChannelOf(UInt_t* record)
      { return (record[1] & 0x00780000) >> 19; }
    virtual inline UInt_t NBytesOfDataOf(UInt_t* record)
      { return (LengthOf(record) - 2)*sizeof(UInt_t); }
    virtual inline void CopyDataOf(UInt_t* record, UChar_t* data)
      { memcpy(data, record, NBytesOfDataOf(record)); }
    virtual inline UInt_t NDataOf(UInt_t* record)
      { return NBytesOfDataOf(record); }
    virtual inline int IthDataOf(UInt_t* record, size_t iData) { 
      //int data = (record[iData/4+2] && (0xffff << (3 - iData%4))) >> (3 - iData%4); 
      int data = (((UChar_t*) record) + 8)[iData];
      return data >= 128 ? data-256 : data;
      return data;
    }

    // for event counter
    virtual inline size_t GetEventCount(UInt_t* /*record*/) { return 1; }

    virtual std::string GetDataObjectPath() { return "ORTek754DModel:ScopeData"; }
};

#endif
