// ORLakeshore210TreeWriter.hh

#ifndef _ORLakeshore210TreeWriter_hh_
#define _ORLakeshore210TreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORLakeshore210Decoder.hh"

class ORLakeshore210TreeWriter : public ORVTreeWriter
{
  public:
    ORLakeshore210TreeWriter(std::string treeName = "");
    virtual ~ORLakeshore210TreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() { fIsCelsius = true; fTemperature = 0.; fTime = 0; fChannel = 0;}

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORLakeshore210Decoder* fLakeshore210Decoder;
    Bool_t fIsCelsius;
    Float_t fTemperature;
    UInt_t fTime; 
    UShort_t fChannel;
};

#endif
