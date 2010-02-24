// ORKatrinV4FLTWaveformTreeWriter.hh

#ifndef _ORKatrinV4FLTWaveformTreeWriter_hh_
#define _ORKatrinV4FLTWaveformTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORKatrinV4FLTWaveformDecoder.hh"

class ORKatrinV4FLTWaveformTreeWriter : public ORVTreeWriter
{
  public:
    ORKatrinV4FLTWaveformTreeWriter(std::string treeName = "");
    virtual ~ORKatrinV4FLTWaveformTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fEventID = 0;fCrate = 0; fCard = 0; 
        fChannel = 0; fEnergy = 0; fWaveformLength = 0;
        fResetSec=0; fResetSubSec=0;   //-tb- 2008-02-12
		fChannelMap=0;  //-tb- 2010-02-17
		fEventID=0; fEventFlags=0; fEventInfo=0;  //-tb- 2010-02-17
        fSaveOnlyNonemptyTrees=true; }
    enum EKatrinV4FLTWFTreeWriter{
      //kMaxWFLength = ORKatrinV4FLTWaveformDecoder::kWaveformLength -tb- this was too small
      kMaxWFLength = ORKatrinV4FLTWaveformDecoder::kWaveformLength * 1 /*64*/ //TODO: test it -tb-
      };
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORKatrinV4FLTWaveformDecoder* fEventDecoder;
    UInt_t fSec, fSubSec, fResetSec, fResetSubSec;
    UInt_t fChannelMap;
    UShort_t fCrate, fCard, fChannel;
    UShort_t fWaveform[kMaxWFLength];
    UInt_t fWaveformLength;
    UInt_t fEnergy;
    UInt_t fEventID, fEventFlags, fEventInfo;
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
