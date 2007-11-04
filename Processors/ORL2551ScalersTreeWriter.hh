// ORL2551ScalersTreeWriter.hh

#ifndef _ORL2551ScalersTreeWriter_hh_
#define _ORL2551ScalersTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORL2551ScalersDecoder.hh"

class ORL2551ScalersTreeWriter : public ORVTreeWriter
{
  public:
    ORL2551ScalersTreeWriter(std::string treeName = "");
    virtual ~ORL2551ScalersTreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      { fCrate = 0; fCard = 0; fChannel = 0; fScaler = 0; }

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORL2551ScalersDecoder* fScalersDecoder;
    UInt_t fCrate, fCard, fChannel, fScaler;
};

#endif
