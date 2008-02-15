// ORKatrinFLTWaveformTreeWriter.hh

#ifndef _ORKatrinFLTWaveformTreeWriter_hh_
#define _ORKatrinFLTWaveformTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORKatrinFLTWaveformDecoder.hh"

class ORKatrinFLTWaveformTreeWriter : public ORVTreeWriter
{
  public:
    ORKatrinFLTWaveformTreeWriter(std::string treeName = "");
    virtual ~ORKatrinFLTWaveformTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fEventID = 0;fCrate = 0; fCard = 0; 
        fChannel = 0; fEnergy = 0; fWaveformLength = 0;
        fResetSec=0; fResetSubSec=0;   //-tb- 2008-02-12
        fChannelMap=0; fPageNumber=0; }//-tb- 2008-02-12
    enum EKatrinFLTWFTreeWriter{
      kMaxWFLength = ORKatrinFLTWaveformDecoder::kWaveformLength};
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORKatrinFLTWaveformDecoder* fEventDecoder;
    UInt_t fSec, fSubSec, fResetSec, fResetSubSec;
    UInt_t fEventID, fChannelMap;
    UShort_t fCrate, fCard, fChannel, fPageNumber;
    UShort_t fWaveform[kMaxWFLength];
    Int_t fWaveformLength;
    UInt_t fEnergy;
};

#endif
