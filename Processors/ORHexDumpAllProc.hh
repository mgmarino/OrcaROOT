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
    virtual void SetLimits(Int_t begin, Int_t end=-1) {
      fBegin=begin;
      fEnd=end;
      fCheckLimits=true;
    }

  private:
    std::map<int, std::string> MakeIDMap();

  protected:
    bool fCheckLimits;
    Int_t fBegin;
    Int_t fEnd; //if fEnd<0, output until end of file
};

#endif
