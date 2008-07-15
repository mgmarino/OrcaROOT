// ORNPLpCMeterDecoder.hh

#ifndef _ORNPLpCMeterDecoder_hh_
#define _ORNPLpCMeterDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORNPLpCMeterDecoder : public ORVBasicTreeDecoder
{
  public: 
    ORNPLpCMeterDecoder() {}
    virtual ~ORNPLpCMeterDecoder() {}

    inline UShort_t DeviceNumberOf( UInt_t* record )
      { return record[1] & 0xF; } 
    inline UInt_t UnixTimeOf( UInt_t* record )
      { return record[2]; }
    inline UShort_t IDCounterOf( UInt_t* record )
      { return ( record[3] & 0xFF000000 ) >> 24; } 
    inline UShort_t ChannelOf( UInt_t* record )
      { return ( record[3] & 0x00F00000 ) >> 20; } 
    inline UInt_t DataOf( UInt_t* record )
      { return ( record[3] & 0x000FFFFF ); } 



    virtual size_t GetNPars() { return 7; }
    virtual std::string GetParName( size_t iPar );
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);

    virtual std::string GetDataObjectPath() 
      { return "NplpCMeter:NplpCMeter"; }
};

#endif
