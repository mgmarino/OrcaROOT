// ORTrigger32ClockTreeWriter.hh

#ifndef _ORTrigger32ClockTreeWriter_hh_
#define _ORTrigger32ClockTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "ORTrigger32ClockDecoder.hh"

class ORTrigger32ClockTreeWriter : public ORVTreeWriter
{
  public:
    enum ETriggerBitsFilter { kTrigger1 = 0x1, kTrigger2 = 0x2, kBothTriggers = 0x3 };
  public:
    ORTrigger32ClockTreeWriter(std::string treeName = "");
    virtual ~ORTrigger32ClockTreeWriter();

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() { fTriggerBits = 0; fT_s = 0.0; }
    virtual void SetTriggerBitsFilter(ETriggerBitsFilter filter) { fTriggerBitsFilter = filter; }

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORTrigger32ClockDecoder* fTrigger32ClockDecoder;
    ETriggerBitsFilter fTriggerBitsFilter;
    UInt_t fTriggerBits;
    double fT_s;
};

#endif
