// ORIpeV4FLTEnergyTreeWriter.hh

#ifndef _ORIpeV4FLTEnergyTreeWriter_hh_
#define _ORIpeV4FLTEnergyTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORIpeV4FLTEnergyDecoder.hh"

class ORIpeV4FLTEnergyTreeWriter : public ORVTreeWriter
{
  public:
    ORIpeV4FLTEnergyTreeWriter(std::string treeName = "");
    virtual ~ORIpeV4FLTEnergyTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fCrate = 0; fCard = 0; fChannel = 0; fEnergy = 0; fEventID = 0;
        fSaveOnlyNonemptyTrees=true; }
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORIpeV4FLTEnergyDecoder* fEventDecoder;
   UInt_t fSec, fSubSec;
   UShort_t fEventID, fChannelMap, fPageNumber;
    UShort_t fCrate, fCard, fChannel;
    UInt_t fEnergy, fResetSec, fResetSubSec;
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
