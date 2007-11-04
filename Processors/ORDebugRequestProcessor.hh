// ORDebugRequestProcessor.hh

#ifndef _ORDebugRequestProcessor_hh_
#define _ORDebugRequestProcessor_hh_

#include "ORVOrcaRequestProcessor.hh"
#include <vector>

class ORDebugRequestProcessor : public ORVOrcaRequestProcessor 
{
  public:
    ORDebugRequestProcessor();
    virtual ~ORDebugRequestProcessor();

    // The following two functions are called by the Request Manager to begin
    // procesing.  They must be defined by the derived class.
    virtual const std::string GetNameOfRequestProcessor() {return "ORDebugRequestProcessor";} 
    virtual bool ExecuteProcess();

  protected:
    int fInt;
    double fDouble;
    std::string fString;
    std::vector<int> fIntVector;
    std::vector<double> fDoubleVector;
    std::vector<std::string> fStringVector;
};

#endif
