// ORBasicRDTreeWriter.hh

#ifndef _ORBasicRDTreeWriter_hh_
#define _ORBasicRDTreeWriter_hh_

#include "ORBasicTreeWriter.hh"
#include "ORVBasicRDTreeDecoder.hh"

class ORBasicRDTreeWriter : public ORBasicTreeWriter
{
  public:
    ORBasicRDTreeWriter(ORVBasicRDTreeDecoder* decoder, std::string treeName, std::string branchPrefix="");
    virtual ~ORBasicRDTreeWriter() {}

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual void Clear();

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORVBasicRDTreeDecoder* fRDDecoder;
    double fRefDate;
};

#endif
