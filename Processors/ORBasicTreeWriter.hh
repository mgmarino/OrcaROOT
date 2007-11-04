// ORBasicTreeWriter.hh

#ifndef _ORBasicTreeWriter_hh_
#define _ORBasicTreeWriter_hh_

#include "ORVBasicTreeDecoder.hh"
#include "ORVTreeWriter.hh"

class ORBasicTreeWriter : public ORVTreeWriter
{
  public:
    ORBasicTreeWriter(ORVBasicTreeDecoder* decoder, std::string treeName = "", std::string branchPrefix = "");
    virtual ~ORBasicTreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual void Clear();

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORVBasicTreeDecoder* fBasicTreeDecoder;
    std::vector<UInt_t*> fParameters;
    std::string fBranchPrefix;
};

#endif
