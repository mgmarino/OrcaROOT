// ORIP320ADCDecoder.hh
/*
*/
#ifndef _ORIP320ADCDecoder_hh_
#define _ORIP320ADCDecoder_hh_

#include "ORVBasicTreeDecoder.hh"
#include "ORVIndustryPakDecoder.hh"

class ORIP320ADCDecoder: public ORVBasicTreeDecoder, public ORVIndustryPakDecoder
{
  public:
    ORIP320ADCDecoder() {}
    virtual ~ORIP320ADCDecoder() {}
    enum EORIP320ADCConsts { kBufferHeaderLength = 3,
                             kMaxADCRecordsChannels = 40 };
    
   virtual inline size_t TotalADCValuesOf( UInt_t* record )
      { return ( LengthOf( record ) - kBufferHeaderLength <= 0 ) 
                 ? 0 : (size_t) ( LengthOf( record ) - kBufferHeaderLength ); } 

    //! Returns time since 1 Jan 1970
    virtual UInt_t TimeOf( UInt_t* record )
      { return record[2]; } 
    
    //! Returns channel number at the offset.
    /*!
        \param offset ranges from 0 to TotalADCValuesOf() - 1
     */
    virtual UInt_t ChannelOf( UInt_t* record, size_t offset )
    { return ( record[kBufferHeaderLength + offset] & 0xFF0000 ) >> 16; }

    //! Returns adc value at the offset.
    /*!
        \param offset ranges from 0 to TotalADCValuesOf() - 1
     */
    virtual UInt_t ADCValueOf( UInt_t* record, size_t offset )
    { return ( record[kBufferHeaderLength + offset] & 0xFFF ); }

    /*!
       Parameters are as follows:

        - Crate
        - Card
        - IP Slot
        - Time
        - Channel
        - ADC Value
     */
    virtual size_t GetNPars() { return 6; }
    virtual std::string GetParName( size_t iPar );
    virtual inline size_t GetNRows( UInt_t* record )
      { return TotalADCValuesOf( record ); }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);
    virtual std::string GetDataObjectPath() 
      { return "ORIP320Model:IP320ADC"; }  
    
};

#endif
