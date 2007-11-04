// ORAcqirisDC440TreeWriter.hh

#ifndef __ORAcqirisDC440TreeWriter_hh__
#define __ORAcqirisDC440TreeWriter_hh__

#include "ORVTreeWriter.hh"
#include "ORAcqirisDC440Decoder.hh"

class ORAcqirisDC440TreeWriter : public ORVTreeWriter
{
  public:
    ORAcqirisDC440TreeWriter(std::string treeName = "");
    virtual ~ORAcqirisDC440TreeWriter();
    enum ORAcqirisDC440TreeWriterConsts{kMaxWFLength = 20000};
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fEventTime = 0.0; fCrate = 0; fCard = 0; fChannel = 0; 
        fWaveformLength = 0; fSamplingPeriod = 0.;}
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORAcqirisDC440Decoder* fEventDecoder;
    Double_t fEventTime;
    Double_t fSamplingPeriod;
    UShort_t fCrate, fCard, fChannel;
    Short_t fWaveform[kMaxWFLength];
    size_t fWaveformLength;
};

#endif
