// ORKatrinFLTEnergyTreeWriter.hh

#ifndef _ORKatrinFLTEnergyTreeWriter_hh_
#define _ORKatrinFLTEnergyTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORKatrinFLTEnergyDecoder.hh"

class ORKatrinFLTEnergyTreeWriter : public ORVTreeWriter
{
  public:
    ORKatrinFLTEnergyTreeWriter(std::string treeName = "");
    virtual ~ORKatrinFLTEnergyTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fCrate = 0; fCard = 0; fChannel = 0; fEnergy = 0; fEventID = 0;
        fSaveOnlyNonemptyTrees=true; }
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORKatrinFLTEnergyDecoder* fEventDecoder;
   UInt_t fSec, fSubSec;
   UShort_t fEventID, fChannelMap, fPageNumber;
    UShort_t fCrate, fCard, fChannel;
    UInt_t fEnergy, fResetSec, fResetSubSec;
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
