// ORIpeV4FLTEnergyHistogramTreeWriterreeWriter.hh

#ifndef _ORIpeV4FLTEnergyHistogramTreeWriter_hh_
#define _ORIpeV4FLTEnergyHistogramTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORIpeV4FLTEnergyHistogramDecoder.hh"

class ORIpeV4FLTEnergyHistogramTreeWriter : public ORVTreeWriter
{
  public:
    ORIpeV4FLTEnergyHistogramTreeWriter(std::string treeName = "");
    virtual ~ORIpeV4FLTEnergyHistogramTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      {  fCrate = 0; fCard = 0; fChannel = 0;
         fReadoutSec = 0; fRefreshTime = 0; fFirstBin = 0; fLastBin = 0; fHistogramLength = 0; 
         fMaxHistogramLength = 0; fBinSize = 0; fOffsetEMin = 0;
         fHistID = 0;  fHistInfo= 0; 
         fSaveOnlyNonemptyTrees=true; }
    enum EIpeV4FLTEHistTreeWriter{
      kMaxHistoLength = 2048
      };
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORIpeV4FLTEnergyHistogramDecoder* fEventDecoder;
    UShort_t fCrate, fCard, fChannel;
    UInt_t fReadoutSec,
    fRefreshTime,
    fFirstBin,
    fLastBin,
    fHistogramLength,
    fMaxHistogramLength,
    fBinSize,
    fOffsetEMin,
    fHistID,
    fHistInfo;
    UInt_t fHistogram[kMaxHistoLength];

};

#endif
