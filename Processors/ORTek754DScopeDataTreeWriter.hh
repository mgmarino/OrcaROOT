// ORTek754DScopeDataTreeWriter.hh

#ifndef _ORTek754DScopeDataTreeWriter_hh_
#define _ORTek754DScopeDataTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORTek754DScopeDataDecoder.hh"

class ORTek754DScopeDataTreeWriter : public ORVTreeWriter
{
  public:
    enum EScopeDataTreeWriterConsts { kMaxNData = 32768 };
  public:
    ORTek754DScopeDataTreeWriter(std::string treeName = "");
    ORTek754DScopeDataTreeWriter(ORTek754DScopeDataDecoder* decoder, std::string treeName = "");
    virtual ~ORTek754DScopeDataTreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear()
      { fGPIBAddress = 0; fChannel = 0; fNData = 0; }

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORTek754DScopeDataDecoder* fScopeDataDecoder;
    int fGPIBAddress, fChannel, fNData;
    int fData[kMaxNData];
    int fI[kMaxNData];
    bool fOwnsDecoder;
};

#endif
