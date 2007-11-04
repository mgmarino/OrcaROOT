// ORDGF4cMCAWriter.hh

#ifndef _ORDGF4cMCAWriter_hh_
#define _ORDGF4cMCAWriter_hh_

#include <string>
#include "ORDataProcessor.hh"
#include "ORDGF4cMCADecoder.hh"

class ORDGF4cMCAWriter : public ORDataProcessor
{
  public:
    ORDGF4cMCAWriter(std::string histrogramName = "ORDGF4cMCAData");
    virtual ~ORDGF4cMCAWriter() {}
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    std::string fHistNameBase;
    ORDGF4cMCADecoder* fMCADecoder;
};

#endif
