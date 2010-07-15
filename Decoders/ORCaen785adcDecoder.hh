// ORCaen785adcDecoder.hh

#ifndef _ORCaen785adcDecoder_hh_
#define _ORCaen785adcDecoder_hh_

#include "ORCaen792qdcDecoder.hh"

class ORCaen785adcDecoder : public ORCaen792qdcDecoder
{
  public:
    ORCaen785adcDecoder() {}
    virtual ~ORCaen785adcDecoder() {}

    virtual std::string GetDataObjectPath() { return "ORCaen785Model:adc"; }
    virtual std::string GetValueName() { return "Adc"; }
};

class ORCaen785NtdcDecoder : public ORCaen785adcDecoder
{
public:
	virtual inline UInt_t IthChannelOf(UInt_t* record, size_t iRow)
	{ return (GetLocPtr(record, iRow)[0] & 0x001E0000) >> 17; }
	
	virtual std::string GetDataObjectPath() { return "ORCaen785Model:AdcN"; }
    virtual std::string GetValueName() { return "AdcN"; }
};

#endif