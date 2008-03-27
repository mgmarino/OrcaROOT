#ifndef _ORVIndustryPakDecoder_hh_
#define _ORVIndustryPakDecoder_hh_

#include "ORVDataDecoder.hh"

class ORVIndustryPakDecoder: virtual public ORVDataDecoder
{
  public:
    ORVIndustryPakDecoder() {}
    ~ORVIndustryPakDecoder() {}

    virtual UInt_t IPSlotNumberOf( UInt_t* record )
      { return (record[1] & 0xF); }
};

#endif
