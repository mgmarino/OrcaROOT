// ORTrig4ChanTreeWriter.hh

#ifndef __ORTrig4ChanTreeWriter_hh__
#define __ORTrig4ChanTreeWriter_hh__

#include "ORVTreeWriter.hh"
#include "ORTrig4ChanDecoder.hh"

class ORTrig4ChanTreeWriter : public ORVTreeWriter
{
  public:
    ORTrig4ChanTreeWriter(std::string treeName = "");
    virtual ~ORTrig4ChanTreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
   virtual inline void Clear() { ftrig = 0; fUpClock = 0, fLowClock = 0, fClock = 0; }

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORTrig4ChanDecoder* fTrig4ChanDecoder;
    UInt_t ftrig, fUpClock; 
    UInt_t fLowClock;
    ULong64_t fClock;
};

#endif
