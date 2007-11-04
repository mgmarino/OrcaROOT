// ORGretaDumper.hh

#ifndef _ORGretaDumper_hh_
#define _ORGretaDumper_hh_

#include "ORDataProcessor.hh"
#include "ORGretaDecoder.hh"

class ORGretaDumper : public ORDataProcessor
{
  public:
    ORGretaDumper();
    virtual ~ORGretaDumper();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    ORGretaDecoder* fEventDecoder;
};

#endif
