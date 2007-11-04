// ORAugerFLTWaveformTreeWriter.hh

#ifndef _ORAugerFLTWaveformTreeWriter_hh_
#define _ORAugerFLTWaveformTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORAugerFLTWaveformDecoder.hh"

class ORAugerFLTWaveformTreeWriter : public ORVTreeWriter
{
  public:
    ORAugerFLTWaveformTreeWriter(std::string treeName = "");
    virtual ~ORAugerFLTWaveformTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fEventID = 0;fCrate = 0; fCard = 0; 
        fChannel = 0; fEnergy = 0; fWaveformLength = 0;}
    enum EAugerFLTWFTreeWriter{
      kMaxWFLength = ORAugerFLTWaveformDecoder::kWaveformLength};
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORAugerFLTWaveformDecoder* fEventDecoder;
    UInt_t fSec, fSubSec;
    UInt_t fEventID;
    UShort_t fCrate, fCard, fChannel;
    UShort_t fWaveform[kMaxWFLength];
    Int_t fWaveformLength;
    UInt_t fEnergy;
};

#endif
