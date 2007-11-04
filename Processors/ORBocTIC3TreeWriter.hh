// ORBocTIC3TreeWriter.hh

#ifndef _ORBocTIC3TreeWriter_hh_
#define _ORBocTIC3TreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORBocTIC3Decoder.hh"

class ORBocTIC3TreeWriter : public ORVTreeWriter
{
  public:
    ORBocTIC3TreeWriter(std::string treeName = "");
    virtual ~ORBocTIC3TreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() { fPressure = 0.; fTime = 0; fChannel = 0; fDeviceID = 0;}

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORBocTIC3Decoder* fBocTIC3Decoder;
    Float_t fPressure;
    UInt_t fTime; 
    UShort_t fDeviceID;
    UShort_t fChannel;
};

#endif
