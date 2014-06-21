// OREdelweissSLTFLTEventTreeWriter.hh

#ifndef _OREdelweissSLTFLTEventTreeWriter_hh_
#define _OREdelweissSLTFLTEventTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "OREdelweissSLTFLTEventDecoder.hh"

class OREdelweissSLTFLTEventTreeWriter : public ORVTreeWriter
{
  public:
    OREdelweissSLTFLTEventTreeWriter(std::string treeName = "");
    virtual ~OREdelweissSLTFLTEventTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fTimeStamp = 0; fEventID = 0;fCrate = 0; fCard = 0; fFiber =0;
        fChannel = 0; fTrigChannel = 0; fEnergy = 0; fWaveformLength = 0;
		fChannelMap=0;  //-tb- 2010-02-17
		fEventID=0; fEventFlags=0; fEventInfo=0;  //-tb- 2010-02-17
        fSaveOnlyNonemptyTrees=true; }
    enum EEdelweissSLTWFTreeWriter{
      //kMaxWFLength = OREdelweissSLTFLTEventDecoder::kWaveformLength -tb- this was too small
      kMaxWFLength = OREdelweissSLTFLTEventDecoder::kWaveformLength * 1 /*64*/ //TODO: test it -tb-
      };
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    OREdelweissSLTFLTEventDecoder* fEventDecoder;
    UInt_t fSec, fSubSec;
    ULong64_t fTimeStamp;
    UInt_t fChannelMap;
    UShort_t fCrate, fCard, fFiber, fChannel, fTrigChannel;
    UShort_t fWaveform[kMaxWFLength];
    UInt_t fWaveformLength;
    Int_t fEnergy;
    Short_t fTriggerAddr;
    UInt_t fEventID, fEventFlags, fEventInfo;
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
