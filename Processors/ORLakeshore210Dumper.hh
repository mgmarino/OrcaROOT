// ORLakeshore210Dumper.hh

#ifndef _ORLakeshore210Dumper_hh_
#define _ORLakeshore210Dumper_hh_

#include "ORDataProcessor.hh"
#include "ORLakeshore210Decoder.hh"

class ORLakeshore210Dumper : public ORDataProcessor
{
  public:
    ORLakeshore210Dumper();
    virtual ~ORLakeshore210Dumper();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    ORLakeshore210Decoder* fLakeshore210Decoder;
};

#endif
