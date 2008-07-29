// ORKatrinFLTEnergyHistogramTreeWriterreeWriter.hh

#ifndef _ORKatrinFLTEnergyHistogramTreeWriter_hh_
#define _ORKatrinFLTEnergyHistogramTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORKatrinFLTEnergyHistogramDecoder.hh"

class ORKatrinFLTEnergyHistogramTreeWriter : public ORVTreeWriter
{
  public:
    ORKatrinFLTEnergyHistogramTreeWriter(std::string treeName = "");
    virtual ~ORKatrinFLTEnergyHistogramTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      {  fCrate = 0; fCard = 0; fChannel = 0;
         fReadoutSec = 0; fRefreshTime = 0; fFirstBin = 0; fLastBin = 0; fHistogramLength = 0; 
         fMaxHistogramLength = 0; fBinSize = 0; fOffsetEMin = 0;
         fSaveOnlyNonemptyTrees=true; }
    enum EKatrinFLTEHistTreeWriter{
      kMaxHistoLength = 512
      };
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORKatrinFLTEnergyHistogramDecoder* fEventDecoder;
    UShort_t fCrate, fCard, fChannel;
    UInt_t fReadoutSec,
    fRefreshTime,
    fFirstBin,
    fLastBin,
    fHistogramLength,
    fMaxHistogramLength,
    fBinSize,
    fOffsetEMin;
    UInt_t fHistogram[kMaxHistoLength];

};

#endif
