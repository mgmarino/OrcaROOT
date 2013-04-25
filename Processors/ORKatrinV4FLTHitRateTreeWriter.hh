// ORKatrinV4FLTHitrateTreeWriter.hh

#ifndef _ORKatrinV4FLTHitrateTreeWriter_hh_
#define _ORKatrinV4FLTHitrateTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORKatrinV4FLTHitRateDecoder.hh"

class ORKatrinV4FLTHitRateTreeWriter : public ORVTreeWriter
{
  public:
    ORKatrinV4FLTHitRateTreeWriter(std::string treeName = "");
    //virtual ~ORKatrinV4FLTHitRateTreeWriter();
    
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSecond = 0;  fCrate = 0; fCard = 0; fVersion = 0; fNChannels = 0;  fHitrateLength=0; fTotalHitrate=0;
        fSaveOnlyNonemptyTrees=true; }
    enum EKatrinV4FLTHitrateTreeWriter{
      //kMaxWFLength = OREdelweissSLTWaveformDecoder::kWaveformLength -tb- this was too small
      kNumChan =  24
      };
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORKatrinV4FLTHitRateDecoder* fEventDecoder;
    UShort_t fCrate, fCard;
    UShort_t fVersion;
    UInt_t fNChannels, fSecond, fHitrateLength, fTotalHitrate;
    UInt_t fRawData16[kNumChan], fRawData32[kNumChan];
    //UInt_t fHitrateData16[kNumChan], fHitrateData32[kNumChan];
    //array for OV flags and chn
    
    //OLD
   //UInt_t fSec, fSubSec;
   //UShort_t fEventID, fChannelMap, fPageNumber;
   // UInt_t fEnergy, fEventInfo; // removed 2011-06-14 -tb-  fResetSec, fResetSubSec;
    //OLD
    
    
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
