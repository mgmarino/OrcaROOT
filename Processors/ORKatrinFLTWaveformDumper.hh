// ORKatrinFLTWaveformDumper.hh

#ifndef _ORKatrinFLTWaveformDumper_hh_
#define _ORKatrinFLTWaveformDumper_hh_

#include "ORDataProcessor.hh"
#include "ORKatrinFLTWaveformDecoder.hh"

class ORKatrinFLTWaveformDumper : public ORDataProcessor
{
  public:
    ORKatrinFLTWaveformDumper();
    virtual ~ORKatrinFLTWaveformDumper();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    ORKatrinFLTWaveformDecoder* fKatrinFLTWaveformDecoder;
};

#endif
