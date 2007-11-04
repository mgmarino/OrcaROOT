// ORAugerFLTEnergyTreeWriter.hh

#ifndef _ORAugerFLTEnergyTreeWriter_hh_
#define _ORAugerFLTEnergyTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORAugerFLTEnergyDecoder.hh"

class ORAugerFLTEnergyTreeWriter : public ORVTreeWriter
{
  public:
    ORAugerFLTEnergyTreeWriter(std::string treeName = "");
    virtual ~ORAugerFLTEnergyTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fCrate = 0; fCard = 0; fChannel = 0; fEnergy = 0; fEventID = 0.;}
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORAugerFLTEnergyDecoder* fEventDecoder;
    UInt_t fSec, fSubSec, fEventID;
    UShort_t fCrate, fCard, fChannel;
    UInt_t fEnergy;
};

#endif
