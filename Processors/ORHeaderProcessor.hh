// ORHeaderProcessor.hh

#ifndef _ORHeaderProcessor_hh_
#define _ORHeaderProcessor_hh_

#include "ORDataProcessor.hh"
#include "ORHeaderDecoder.hh"
#include "ORHeader.hh"


class ORHeaderProcessor : public ORDataProcessor
{
  public:
    ORHeaderProcessor();
    virtual ~ORHeaderProcessor();

    // overloaded from ORDataProcessor
    virtual EReturnCode ProcessDataRecord(UInt_t* record);

    virtual inline ORHeader* GetHeader() { return &fHeader; }


  protected:
    ORHeaderDecoder* fHeaderDecoder;
    ORHeader fHeader;
    UInt_t fByteCount;
};

#endif
