// ORDataGenBurstTreeWriter.hh

#ifndef _ORDataGenBurstTreeWriter_hh_
#define _ORDataGenBurstTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORDataGenBurstDataDecoder.hh"

class ORDataGenBurstTreeWriter : public ORVTreeWriter
{
  public:
    ORDataGenBurstTreeWriter(std::string treeName = "");
    virtual ~ORDataGenBurstTreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fCrate = 0; fCard = 0; fChannel = 0; fValue = 0;  fTime = 0; }

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORDataGenBurstDataDecoder* fBurstDecoder;
    UShort_t fCrate, fCard, fChannel, fValue;
	UInt_t fTime;
	UInt_t fLastTime,fDiffTime;
};

#endif
