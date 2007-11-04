// ORDGF4cWaveformTreeWriter.hh

#ifndef _ORDGF4cWaveformTreeWriter_hh_
#define _ORDGF4cWaveformTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORDGF4cEventDecoder.hh"

class ORDGF4cWaveformTreeWriter : public ORVTreeWriter
{
  public:
    ORDGF4cWaveformTreeWriter(std::string treeName = "");
    virtual ~ORDGF4cWaveformTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fEventTime = 0.0; fCrate = 0; fCard = 0; fChannel = 0; fEnergy = 0; fWaveformLength = 0;}
    enum EDGF4cWFTreeWriter{kMaxWFLength = 4000};
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORDGF4cEventDecoder* fEventDecoder;
    Double_t fEventTime;
    UShort_t fCrate, fCard, fChannel;
    UShort_t fWaveform[kMaxWFLength];
    UShort_t fEnergy;
    Int_t fWaveformLength;
};

#endif
