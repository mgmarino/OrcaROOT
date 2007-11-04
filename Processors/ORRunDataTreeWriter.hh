// ORRunDataTreeWriter.hh

#ifndef _ORRunDataTreeWriter_hh_
#define _ORRunDataTreeWriter_hh_

#include "ORBasicTreeWriter.hh"
#include "ORRunDecoder.hh"

class ORRunDataTreeWriter : public ORBasicTreeWriter
{
  public:
    ORRunDataTreeWriter(std::string treeName = "RunDataTree");
    virtual ~ORRunDataTreeWriter();

    // overloaded from ORBasicTreeWriter
    virtual EReturnCode ProcessDataRecord(UInt_t* record);
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual EReturnCode InitializeBranches();

  protected:
    ORRunDecoder* fRunDecoder;
    UInt_t fByteCount;
};

#endif
