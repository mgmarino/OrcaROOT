// ORHexDumpAllProc.hh

#ifndef _ORHexDumpAllProc_hh_
#define _ORHexDumpAllProc_hh_

#include "ORDataProcessor.hh"

class ORHexDumpAllProc : public ORDataProcessor
{
  public:
    ORHexDumpAllProc();
    virtual ~ORHexDumpAllProc();

    virtual void SetDataId() {}
    EReturnCode ProcessDataRecord(UInt_t* record);

  private:
    std::map<int, std::string> MakeIDMap();
};

#endif
