// ORAmi286TreeWriter.hh

#ifndef _ORAmi286TreeWriter_hh_
#define _ORAmi286TreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORAmi286Decoder.hh"

class ORAmi286TreeWriter : public ORVTreeWriter
{
  public:
    ORAmi286TreeWriter(std::string treeName = "");
    virtual ~ORAmi286TreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() { fStatus = 0.; fLevel = 0.; fTime = 0; fChannel = 0; fDeviceID = 0;}

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORAmi286Decoder* fAmi286Decoder;
    Float_t fLevel;
    UInt_t fTime; 
    UShort_t fDeviceID;
    UShort_t fChannel;
    UShort_t fStatus;
};

#endif
