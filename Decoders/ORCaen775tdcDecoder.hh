// ORCaen775tdcDecoder.hh

#ifndef _ORCaen775tdcDecoder_hh_
#define _ORCaen775tdcDecoder_hh_

#include "ORCaen792qdcDecoder.hh"

class ORCaen775tdcDecoder : public ORCaen792qdcDecoder
{
  public:
    ORCaen775tdcDecoder() {}
    virtual ~ORCaen775tdcDecoder() {}

    virtual std::string GetDataObjectPath() { return "ORCaen775Model:Tdc"; }
    virtual std::string GetValueName() { return "Tdc"; }
};


class ORCaen775NtdcDecoder : public ORCaen775tdcDecoder
{
public:
	virtual inline UInt_t IthChannelOf(UInt_t* record, size_t iRow)
	{ return (GetLocPtr(record, iRow)[0] & 0x001E0000) >> 17; }
	
	virtual std::string GetDataObjectPath() { return "ORCaen775Model:TdcN"; }
    virtual std::string GetValueName() { return "TdcN"; }
};

#endif
