// ORDGF4cEnergyTreeWriter.hh

#ifndef _ORDGF4cEnergyTreeWriter_hh_
#define _ORDGF4cEnergyTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORDGF4cEventDecoder.hh"

class ORDGF4cEnergyTreeWriter : public ORVTreeWriter
{
  public:
    ORDGF4cEnergyTreeWriter(std::string treeName = "");
    virtual ~ORDGF4cEnergyTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fEventTime = 0.0; fCrate = 0; fCard = 0; fChannel = 0; fEnergy = 0; }

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORDGF4cEventDecoder* fEventDecoder;
    double fEventTime;
    UShort_t fCrate, fCard, fChannel;
    short fEnergy;
};

#endif
