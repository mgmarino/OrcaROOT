// OROrcaRequestProcessor.hh

#ifndef _OROrcaRequestProcessor_hh_
#define _OROrcaRequestProcessor_hh_

#include "ORDataProcessor.hh"
#include "OROrcaRequestDecoder.hh"
#include "ORVOrcaRequestProcessor.hh"
#include <string>
#include <map>

class OROrcaRequestProcessor : public ORDataProcessor
{
  public:
    OROrcaRequestProcessor();
    virtual ~OROrcaRequestProcessor();

    // overloaded from ORDataProcessor
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);


  protected:
    virtual bool LoadInputs();
    virtual bool LoadOutputs();
    virtual bool ExecuteProcess();
    virtual bool LoadRequestHandler(const std::string&);
    virtual bool ExecuteAll(UInt_t* record);
    virtual void SendErrorToOrca();
    OROrcaRequestDecoder* fOrcaRequestDecoder;
    std::map<std::string, ORVOrcaRequestProcessor*> fReqProcessorMap;
    ORVOrcaRequestProcessor* fCurrentReqProcessor;
};

#endif
