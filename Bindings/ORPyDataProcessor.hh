// ORPyDataProcessor.hh

#ifndef _ORPyDataProcessor_hh_
#define _ORPyDataProcessor_hh_

#include "ORPyBinder.hh"
#include "ORDataProcessor.hh"

class ORPyDataProcessor : public ORDataProcessor, public ORPyBinder
{
  public:
    ORPyDataProcessor(ORVDataDecoder* dec = 0) : ORDataProcessor(dec) {}
   
    // these are the functions that are typically overloaded
    virtual EReturnCode StartProcessing();
    virtual EReturnCode StartRun();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual EReturnCode EndRun();
    virtual EReturnCode EndProcessing();

    ClassDef(ORPyDataProcessor, 0)

};

#endif
