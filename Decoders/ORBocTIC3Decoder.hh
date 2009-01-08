// ORBocTIC3Decoder.hh
/*
*/
#ifndef _ORBocTIC3Decoder_hh_
#define _ORBocTIC3Decoder_hh_

#include "ORVBasicTreeDecoder.hh"
#include <string>

class ORBocTIC3Decoder: public ORVBasicTreeDecoder
{
  public:
    ORBocTIC3Decoder() {}
    virtual ~ORBocTIC3Decoder() {}
    enum EORBocTIC3Consts {kNumChannels = 3};
    
    virtual inline UInt_t GetNumberOfChannels() {return (UInt_t)kNumChannels;}
    /* The number of channels in this card. */

    virtual inline UShort_t GetDeviceID(UInt_t* record) {return record[1] & 0xfff;} 
    /* Return the id of the device. */

    virtual Float_t GetPressureOfChannel(UInt_t* record, UInt_t channel); 
    /* Pressure of a channel. */

    virtual UInt_t GetTimeOfChannel(UInt_t* record, UInt_t channel); 
    /* Time in channel given in seconds since 1 Jan 1970. */

    virtual void Dump(UInt_t* record);
    /* Provides a debug dump of the record. */

    virtual std::string GetDataObjectPath() 
      { return "BocTIC3Model:Pressures"; }  

    /* Satisfying the ORVBasicTreeDecoder interface. */
    virtual size_t GetNPars() { return 3; } 
    virtual size_t GetNRows(UInt_t* /*record*/) { return GetNumberOfChannels(); }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);
    
};

#endif
