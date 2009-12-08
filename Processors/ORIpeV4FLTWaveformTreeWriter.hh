// ORIpeV4FLTWaveformTreeWriter.hh

#ifndef _ORIpeV4FLTWaveformTreeWriter_hh_
#define _ORIpeV4FLTWaveformTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORIpeV4FLTWaveformDecoder.hh"

class ORIpeV4FLTWaveformTreeWriter : public ORVTreeWriter
{
  public:
    ORIpeV4FLTWaveformTreeWriter(std::string treeName = "");
    virtual ~ORIpeV4FLTWaveformTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fEventID = 0;fCrate = 0; fCard = 0; 
        fChannel = 0; fEnergy = 0; fWaveformLength = 0;
        fResetSec=0; fResetSubSec=0;   //-tb- 2008-02-12
        fChannelMap=0; fPageNumber=0; //-tb- 2008-02-12
        fSaveOnlyNonemptyTrees=true; }
    enum EIpeV4FLTWFTreeWriter{
      //kMaxWFLength = ORIpeV4FLTWaveformDecoder::kWaveformLength -tb- this was too small
      kMaxWFLength = ORIpeV4FLTWaveformDecoder::kWaveformLength * 64
      };
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORIpeV4FLTWaveformDecoder* fEventDecoder;
    UInt_t fSec, fSubSec, fResetSec, fResetSubSec;
    UShort_t fEventID, fChannelMap;
    UShort_t fCrate, fCard, fChannel, fPageNumber;
    UShort_t fWaveform[kMaxWFLength];
    UInt_t fWaveformLength;
    UInt_t fEnergy;
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
