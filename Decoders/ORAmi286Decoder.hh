// ORAmi286Decoder.hh
/*
 //------------------------------------------------------------------------------------------------
 // Data Format
 //xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
 //^^^^ ^^^^ ^^^^ ^^-----------------------data id
 //                 ^^ ^^^^ ^^^^ ^^^^ ^^^^-length in longs
 //
 // xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
 // ^^^^------------------------------------ fill state for level 3
 //      ^^^^------------------------------- fill state for level 2
 //           ^^^^-------------------------- fill state for level 1
 //                ^^^^--------------------- fill state for level 0
 //                          ^^^^ ^^^^ ^^^^- device id
 // xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  level chan 0 encoded as a float
 // xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  time level 0 taken in seconds since Jan 1, 1970
 // xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  level chan 1 encoded as a float
 // xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  time level 1 taken in seconds since Jan 1, 1970
 // xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  level chan 2 encoded as a float
 // xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  time level 2 taken in seconds since Jan 1, 1970
 // xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  level chan 3 encoded as a float
 // xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  time level 3 taken in seconds since Jan 1, 1970
 //-----------------------------------------------------------------------------------------------
 
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
  
    virtual UShort_t GetStatusOfChannel(UInt_t* record, UInt_t channel);
    /* Status of the solenoid valve */
  
    virtual void Dump(UInt_t* record);
    /* Provides a debug dump of the record. */

    virtual std::string GetDataObjectPath() 
      { return "Ami286Model:Levels"; }  
    
};

#endif
