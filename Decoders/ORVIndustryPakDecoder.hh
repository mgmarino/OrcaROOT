#ifndef _ORVIndustryPakDecoder_hh_
#define _ORVIndustryPakDecoder_hh_

#include "ORVDataDecoder.hh"
//! class defining a simple interface for IndustryPak cards
class ORVIndustryPakDecoder: virtual public ORVDataDecoder
{
  public:
    ORVIndustryPakDecoder() {}
    ~ORVIndustryPakDecoder() {}

    //! Returns time since 1 Jan 1970
    virtual UInt_t TimeOf( UInt_t* record )
      { return record[2]; } 

    //! Returns the slot number of the Industry Pak card
    virtual UInt_t IPSlotNumberOf( UInt_t* record )
      { return (record[1] & 0xF); }
};

#endif
