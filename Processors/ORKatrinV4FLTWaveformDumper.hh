// ORKatrinV4FLTWaveformDumper.hh

#ifndef _ORKatrinV4FLTWaveformDumper_hh_
#define _ORKatrinV4FLTWaveformDumper_hh_

#include "ORDataProcessor.hh"
#include "ORKatrinV4FLTWaveformDecoder.hh"

class ORKatrinV4FLTWaveformDumper : public ORDataProcessor
{
  public:
    ORKatrinV4FLTWaveformDumper();
    virtual ~ORKatrinV4FLTWaveformDumper();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    ORKatrinV4FLTWaveformDecoder* fKatrinV4FLTWaveformDecoder;
};

#endif
