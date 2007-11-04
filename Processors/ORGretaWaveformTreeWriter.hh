// ORGretaWaveformTreeWriter.hh

#ifndef _ORGretaWaveformTreeWriter_hh_
#define _ORGretaWaveformTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORGretaDecoder.hh"

class ORGretaWaveformTreeWriter : public ORVTreeWriter
{
  public:
    ORGretaWaveformTreeWriter(std::string treeName = "");
    virtual ~ORGretaWaveformTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fLEDEventTime = 0.0; fCFDEventTime = 0.0; fCrate = 0; fCard = 0; fChannel = 0; fEnergy = 0; fWaveformLength = 0;}
    enum EDGF4cWFTreeWriter{kMaxWFLength = 5000};
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORGretaDecoder* fEventDecoder;
    Double_t fLEDEventTime;
    Double_t fCFDEventTime;
    UShort_t fCrate, fCard, fChannel,fBoard;
    UShort_t fWaveform[kMaxWFLength];
    size_t fWaveformLength;
    UInt_t fEnergy;
};

#endif
