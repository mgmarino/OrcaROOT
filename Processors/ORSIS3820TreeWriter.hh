// ORSIS3820TreeWriter.hh

#ifndef _ORSIS3820TreeWriter_hh_
#define _ORSIS3820TreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORSIS3820Decoder.hh"

class ORSIS3820TreeWriter : public ORVTreeWriter
{
  public:
    ORSIS3820TreeWriter(std::string treeName = "");
    virtual ~ORSIS3820TreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fCrate = 0; fCard = 0; fChannel = 0; fScaler = 0; }

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORSIS3820Decoder* fScalersDecoder;
    UInt_t fCrate, fCard, fChannel, fScaler;
};

#endif
