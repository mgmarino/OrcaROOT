// ORHistWriter.hh

#ifndef _ORHistWriter_hh_
#define _ORHistWriter_hh_

#include <map>
#include "TH1.h"
#include "ORDataProcessor.hh"
#include "ORVHistDecoder.hh"


class ORHistWriter : public ORDataProcessor
{
  public:
    ORHistWriter(ORVHistDecoder* histDecoder);
    virtual ~ORHistWriter() {}

    virtual EReturnCode StartProcessing();
    virtual EReturnCode StartRun();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual EReturnCode EndRun();
    virtual EReturnCode EndProcessing();

  protected:
    std::map<int, TH1*> fHists;
    ORVHistDecoder* fHistDecoder;
};

#endif
