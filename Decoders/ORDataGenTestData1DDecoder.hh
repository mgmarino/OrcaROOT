// ORDataGenTestData1DDecoder.hh

#ifndef _ORDataGenTestData1DDecoder_hh_
#define _ORDataGenTestData1DDecoder_hh_

#include <string>
#include "ORVDataDecoder.hh"

class ORDataGenTestData1DDecoder: public ORVDataDecoder
{
  public:
    ORDataGenTestData1DDecoder() {}
    virtual ~ORDataGenTestData1DDecoder() {}

    virtual std::string GetDataObjectPath() 
      { return "ORDataGenModel:"+GetDataGenType(); }

    virtual std::string GetDataGenType() { return "TestData1D"; }

    virtual UShort_t GetCard(UInt_t* dataRecord) 
      { return (dataRecord[1] & 0x000F0000) >> 16; }

    virtual UShort_t GetChannel(UInt_t* dataRecord) 
      { return (dataRecord[1] & 0x0000F000) >> 12; }

    virtual UShort_t GetValue(UInt_t* dataRecord) 
      { return (dataRecord[1] & 0x00000FFF); }

};

#endif
