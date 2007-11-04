// ORShaperShaperTreeWriter.hh

#ifndef _ORShaperShaperTreeWriter_hh_
#define _ORShaperShaperTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORShaperShaperDecoder.hh"

class ORShaperShaperTreeWriter : public ORVTreeWriter
{
  public:
    ORShaperShaperTreeWriter(std::string treeName = "");
    virtual ~ORShaperShaperTreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear()
      { fCrate = 0; fCard = 0; fChannel = 0; fADC = 0; }

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORShaperShaperDecoder* fShaperShaperDecoder;
    int fCrate, fCard, fChannel, fADC;
};

#endif
