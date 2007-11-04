// ORBocTIC3Dumper.hh

#ifndef _ORBocTIC3Dumper_hh_
#define _ORBocTIC3Dumper_hh_

#include "ORDataProcessor.hh"
#include "ORBocTIC3Decoder.hh"

class ORBocTIC3Dumper : public ORDataProcessor
{
  public:
    ORBocTIC3Dumper();
    virtual ~ORBocTIC3Dumper();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    ORBocTIC3Decoder* fBocTIC3Decoder;
};

#endif
