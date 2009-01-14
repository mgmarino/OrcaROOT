// ORMksPdr2000Decoder.hh
/*
 * Decoder for MksPdr2000 which measures pressure and time.  It is very similar
 * to the BocTIC3 
 */
#ifndef _ORMksPdr2000Decoder_hh_
#define _ORMksPdr2000Decoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORMksPdr2000Decoder: public ORVBasicTreeDecoder
{
  public:
    ORMksPdr2000Decoder() {}
    virtual ~ORMksPdr2000Decoder() {}
    enum EORMksPdr2000Consts {kNumChannels = 3};
    enum EORMksPdr2000Units { kUnknown = 0,
                              kTorr,
                              kmBar,
                              kPascal,
                              kArbitrary };
    
    virtual inline EORMksPdr2000Units GetUnits(UInt_t* record) 
      {return (EORMksPdr2000Units)((record[1] >> 16) & 0xf);} 

    /* The number of channels in this card. */
    virtual inline UInt_t GetNumberOfChannels() {return (UInt_t)kNumChannels;}
 
    virtual inline UShort_t GetDeviceID(UInt_t* record) {return record[1] & 0xfff;} 
    /* Return the id of the device. */

    virtual Float_t GetPressureOfChannel(UInt_t* record, UInt_t channel); 
    /* Pressure of a channel. */

    virtual UInt_t GetTimeOfChannel(UInt_t* record, UInt_t channel); 
    /* Time in channel given in seconds since 1 Jan 1970. */

    virtual void Dump(UInt_t* record);
    /* Provides a debug dump of the record. */


    virtual std::string GetDataObjectPath() 
      { return "MksPdr2000Model:Pressures"; }  
    
    /* Satisfying the ORVBasicTreeDecoder interface. */
    virtual size_t GetNPars() { return 5; } 
    virtual size_t GetNRows(UInt_t* /*record*/) { return GetNumberOfChannels(); }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);
};

#endif
