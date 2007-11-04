// ORLakeshore210Decoder.hh
/*
*/
#ifndef _ORLakeshore210Decoder_hh_
#define _ORLakeshore210Decoder_hh_

#include "ORVDataDecoder.hh"
#include <string>

class ORLakeshore210Decoder: public ORVDataDecoder
{
  public:
    ORLakeshore210Decoder() {}
    virtual ~ORLakeshore210Decoder() {}
    enum EORLakeshore210TempUnits {kKelvin, kCelsius};
    enum EORLakeshore210TempConsts {kNumChannels = 8};
    
    virtual inline EORLakeshore210TempUnits GetTemperatureUnits(UInt_t* record)
      { return (((record[1] & 0x00010000) >> 16) == 0) ? kCelsius : kKelvin;} 
    /* Return the units in which the temperature is given. */

    virtual inline UInt_t GetNumberOfChannels() {return (UInt_t)kNumChannels;}
    /* The number of channels in this card. */
    virtual Float_t GetTempOfChannel(UInt_t* record, UInt_t channel); 
    /* Temperature of a channel. */
    virtual UInt_t GetTimeOfChannel(UInt_t* record, UInt_t channel); 
    /* Time in channel given in seconds since 1 Jan 1970. */

    virtual void Dump(UInt_t* record);
    /* Provides a debug dump of the record. */

    virtual std::string GetDataObjectPath() 
      { return "LakeShore210Model:Temperatures"; }  
    
};

#endif
