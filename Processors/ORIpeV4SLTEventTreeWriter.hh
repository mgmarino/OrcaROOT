// ORIpeV4SLTEventTreeWriter.hh

#ifndef _ORIpeV4SLTEventTreeWriter_hh_
#define _ORIpeV4SLTEventTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORIpeV4SLTEventDecoder.hh"

class ORIpeV4SLTEventTreeWriter : public ORVTreeWriter
{
  public:
    ORIpeV4SLTEventTreeWriter(std::string treeName = "");
    virtual ~ORIpeV4SLTEventTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fEventCounter = 0; fTimestampHigh = 0; fTimestampLow = 0; fCrate = 0; fCard = 0; fEventCounter = 0; }
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORIpeV4SLTEventDecoder* fEventDecoder;
    UShort_t fCrate, fCard;
    UShort_t fCounterType, fCounterSubType;
    UInt_t fEventCounter, fTimestampHigh, fTimestampLow;
    //Bool_t saveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-02-19 - MOVED TO BASE CLASS -tb-
};

#endif
