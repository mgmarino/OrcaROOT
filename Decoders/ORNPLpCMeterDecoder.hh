// ORNPLpCMeterDecoder.hh

#ifndef _ORNPLpCMeterDecoder_hh_
#define _ORNPLpCMeterDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORNPLpCMeterDecoder : public ORVBasicTreeDecoder
{
  public: 
    ORNPLpCMeterDecoder() {}
    virtual ~ORNPLpCMeterDecoder() {}

    enum ENPLpCMeterDecoderConsts { kHeaderSize = 2 };

    inline UShort_t DeviceNumberOf( UInt_t* record )
      { return record[1] & 0xF; } 

    inline UInt_t UnixTimeOf( UInt_t* record )
      { return record[2]; }

    inline UShort_t IDCounterOf( UInt_t* record, size_t iRow )
      { return ( record[3 + iRow] & 0xFF000000 ) >> 24; } 

    inline UShort_t IsSignalOf( UInt_t* record, size_t iRow )
      { return ( record[3 + iRow] & 0x00800000 ) >> 23; } 

    inline UShort_t ChannelOf( UInt_t* record, size_t iRow )
      { return ( record[3 + iRow] & 0x00600000 ) >> 21; } 

    inline UShort_t AOrBOf( UInt_t* record, size_t iRow )
      { return ( record[3 + iRow] & 0x00100000 ) >> 20; } 

    inline UInt_t DataOf( UInt_t* record, size_t iRow )
      { return ( record[3 + iRow] & 0x000FFFFF ); } 

    inline UInt_t NumberDataRecordsOf( UInt_t* record )
      { return ( LengthOf(record) - kHeaderSize - 1);  }


    virtual size_t GetNPars() { return 9; }
    virtual std::string GetParName( size_t iPar );
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);
    virtual size_t GetNRows( UInt_t* record ) 
      { return NumberDataRecordsOf( record ); }

    virtual std::string GetDataObjectPath() 
      { return "NplpCMeter:NplpCMeter"; }
};

#endif
