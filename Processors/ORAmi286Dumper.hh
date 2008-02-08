// ORAmi286Dumper.hh

#ifndef _ORAmi286Dumper_hh_
#define _ORAmi286Dumper_hh_

#include "ORDataProcessor.hh"
#include "ORAmi286Decoder.hh"

class ORAmi286Dumper : public ORDataProcessor
{
  public:
    ORAmi286Dumper();
    virtual ~ORAmi286Dumper();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    ORAmi286Decoder* fAmi286Decoder;
};

#endif
