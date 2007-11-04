// ORTrigger32GTIDTreeWriter.hh

#ifndef _ORTrigger32GTIDTreeWriter_hh_
#define _ORTrigger32GTIDTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORVTrigger32GTIDDecoder.hh"

class ORTrigger32GTIDTreeWriter : public ORVTreeWriter
{
  public:
    ORTrigger32GTIDTreeWriter(ORVTrigger32GTIDDecoder* decoder, std::string treeName = "");
    virtual ~ORTrigger32GTIDTreeWriter() {}

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() { fTriggerBits = 0; fGTID = 0; }

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORVTrigger32GTIDDecoder* fTrigger32GTIDDecoder;
    UInt_t fTriggerBits;
    UInt_t fGTID;
};

#endif
