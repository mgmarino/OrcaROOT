// ORIpeV4FLTWaveformDumper.hh

#ifndef _ORIpeV4FLTWaveformDumper_hh_
#define _ORIpeV4FLTWaveformDumper_hh_

#include "ORDataProcessor.hh"
#include "ORIpeV4FLTWaveformDecoder.hh"

class ORIpeV4FLTWaveformDumper : public ORDataProcessor
{
  public:
    ORIpeV4FLTWaveformDumper();
    virtual ~ORIpeV4FLTWaveformDumper();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    ORIpeV4FLTWaveformDecoder* fIpeV4FLTWaveformDecoder;
};

#endif
