
// OR1DHistoHistogramsDecoder.hh

#ifndef _ORDGF4cMCADecoder_hh_
#define _ORDGF4cMCADecoder_hh_

#include <string>
#include "ORVDataDecoder.hh"

class ORDGF4cMCADecoder : public ORVDataDecoder
{
  public:
    ORDGF4cMCADecoder() {}
    virtual ~ORDGF4cMCADecoder() {}
    virtual inline UInt_t IthBinOf(UInt_t* record, size_t iBin) { return record[iBin+2]; }
      
    virtual std::string GetDataObjectPath() { return "ORDGF4cModel:Waveform"; }
};

#endif
