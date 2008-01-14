
// ORDGF4cLiveTimeWriter.hh

#ifndef _ORDGF4cLiveTimeWriter_hh_
#define _ORDGF4cLiveTimeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORDGF4cLiveTimeDecoder.hh"

class ORDGF4cLiveTimeWriter : public ORVTreeWriter
{
  public:
    ORDGF4cLiveTimeWriter(std::string treeName = "");
    virtual ~ORDGF4cLiveTimeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear(); 
    virtual EReturnCode FindRecordLength();
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORDGF4cLiveTimeDecoder* fLiveTimeDecoder;
    Double_t fRealTime, fRunTime, fLiveTime;
    UShort_t fCrate, fCard, fChannel, fNumEvents;
    Int_t fLength;
};

inline void ORDGF4cLiveTimeWriter::Clear() 
{ 
  fCrate = 0; 
  fCard = 0; 
  fRealTime = 0.0; 
  fRunTime = 0; 
  fChannel = 0; 
  fLiveTime = 0.0; 
  fNumEvents = 0;
}

#endif
