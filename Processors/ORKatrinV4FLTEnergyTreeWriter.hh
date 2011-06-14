// ORKatrinV4FLTEnergyTreeWriter.hh

#ifndef _ORKatrinV4FLTEnergyTreeWriter_hh_
#define _ORKatrinV4FLTEnergyTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORKatrinV4FLTEnergyDecoder.hh"

class ORKatrinV4FLTEnergyTreeWriter : public ORVTreeWriter
{
  public:
    ORKatrinV4FLTEnergyTreeWriter(std::string treeName = "");
    virtual ~ORKatrinV4FLTEnergyTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fSec = 0; fSubSec = 0; fCrate = 0; fCard = 0; fChannel = 0; fEnergy = 0; fEventID = 0;
        fSaveOnlyNonemptyTrees=true; }
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORKatrinV4FLTEnergyDecoder* fEventDecoder;
   UInt_t fSec, fSubSec;
   UShort_t fEventID, fChannelMap, fPageNumber;
    UShort_t fCrate, fCard, fChannel;
    UInt_t fEnergy, fEventInfo; // removed 2011-06-14 -tb-  fResetSec, fResetSubSec;
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
