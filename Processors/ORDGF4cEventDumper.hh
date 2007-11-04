// ORDGF4cEventDumper.hh

#ifndef _ORDGF4cEventDumper_hh_
#define _ORDGF4cEventDumper_hh_

#include "ORDataProcessor.hh"
#include "ORDGF4cEventDecoder.hh"

class ORDGF4cEventDumper : public ORDataProcessor
{
  public:
    ORDGF4cEventDumper();
    virtual ~ORDGF4cEventDumper();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    ORDGF4cEventDecoder* fDGF4cEventDecoder;
};

#endif
