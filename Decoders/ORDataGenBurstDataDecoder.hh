// ORDataGenTestData1DDecoder.hh

#ifndef _ORDataGenTestData1DDecoder_hh_
#define _ORDataGenTestData1DDecoder_hh_

#include <string>
#include "ORVDataDecoder.hh"

class ORDataGenBurstDataDecoder: public ORVDataDecoder
{
  public:
    ORDataGenBurstDataDecoder() {}
    virtual ~ORDataGenBurstDataDecoder() {}

    virtual std::string GetDataObjectPath() 
      { return "ORDataGenModel:"+GetDataGenType(); }

    virtual std::string GetDataGenType() { return "BurstData"; }

    virtual UInt_t CardOf(UInt_t* dataRecord) 
      { return (dataRecord[1] & 0x000F0000) >> 16; }

    virtual UInt_t ChannelOf(UInt_t* dataRecord) 
      { return (dataRecord[1] & 0x0000F000) >> 12; }

    virtual UInt_t ValueOf(UInt_t* dataRecord) 
      { return (dataRecord[1] & 0x00000FFF); }

	virtual UInt_t TimeOf(UInt_t* dataRecord) 
	{ return dataRecord[2]; }

};

#endif
