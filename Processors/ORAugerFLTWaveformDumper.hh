// ORAugerFLTWaveformDumper.hh

#ifndef _ORAugerFLTWaveformDumper_hh_
#define _ORAugerFLTWaveformDumper_hh_

#include "ORDataProcessor.hh"
#include "ORAugerFLTWaveformDecoder.hh"

class ORAugerFLTWaveformDumper : public ORDataProcessor
{
  public:
    ORAugerFLTWaveformDumper();
    virtual ~ORAugerFLTWaveformDumper();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    ORAugerFLTWaveformDecoder* fAugerFLTWaveformDecoder;
};

#endif
