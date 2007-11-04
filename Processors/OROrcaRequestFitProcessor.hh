// OROrcaRequestFitProcessor.hh

#ifndef _OROrcaRequestFitProcessor_hh_
#define _OROrcaRequestFitProcessor_hh_

#include "ORVOrcaRequestProcessor.hh"
#include <vector>

class OROrcaRequestFitProcessor : public ORVOrcaRequestProcessor 
{
  public:
    OROrcaRequestFitProcessor();
    virtual ~OROrcaRequestFitProcessor();

    // The following two functions are called by the Request Manager to begin
    // procesing.  They must be defined by the derived class.
    virtual const std::string GetNameOfRequestProcessor() 
      {return "OROrcaRequestFitProcessor";} 
    virtual bool ExecuteProcess();

  protected:
    int fIntLower;
    int fIntUpper;
    std::string fFitFunc;
    std::string fFitOptions;
    std::vector<double> fYVector;
    std::vector<double> fInputParameters;
    std::vector<double> fOutputParamVector;
    std::vector<double> fOutputParamErrorsVector;
    std::vector<double> fOutputYVector;
    std::vector<std::string> fOutputParamNamesVector;
    std::string fOutputEquation;
    double fOutputChiSquare;
};

#endif
