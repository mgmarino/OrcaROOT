// ORAmi286Decoder.hh
/*
*/
#ifndef _ORAmi286Decoder_hh_
#define _ORAmi286Decoder_hh_

#include "ORVDataDecoder.hh"
#include <string>

class ORAmi286Decoder: public ORVDataDecoder
{
  public:
    ORAmi286Decoder() {}
    virtual ~ORAmi286Decoder() {}
    enum EORAmi286Consts {kNumChannels = 4};
    
    virtual inline UInt_t GetNumberOfChannels() {return (UInt_t)kNumChannels;}
    /* The number of channels in this card. */

    virtual inline UShort_t GetDeviceID(UInt_t* record) {return record[1] & 0xfff;} 
    /* Return the id of the device. */

    virtual Float_t GetLevelOfChannel(UInt_t* record, UInt_t channel); 
    /* Level of a channel. */

    virtual UInt_t GetTimeOfChannel(UInt_t* record, UInt_t channel); 
    /* Time in channel given in seconds since 1 Jan 1970. */

    virtual void Dump(UInt_t* record);
    /* Provides a debug dump of the record. */

    virtual std::string GetDataObjectPath() 
      { return "Ami286Model:Levels"; }  
    
};

#endif
