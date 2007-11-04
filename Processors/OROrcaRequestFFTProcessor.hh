// OROrcaRequestFFTProcessor.hh

#ifdef ORROOT_HAS_FFTW
#ifndef _OROrcaRequestFFTProcessor_hh_
#define _OROrcaRequestFFTProcessor_hh_

#include "ORVOrcaRequestProcessor.hh"
#include <vector>

class OROrcaRequestFFTProcessor : public ORVOrcaRequestProcessor 
{
  public:
    OROrcaRequestFFTProcessor();
    virtual ~OROrcaRequestFFTProcessor();

    // The following two functions are called by the Request Manager to begin
    // procesing.  They must be defined by the derived class.
    virtual const std::string GetNameOfRequestProcessor() 
      {return "OROrcaRequestFFTProcessor";} 
    virtual bool ExecuteProcess();

  protected:
    std::vector<double> fWaveform;
    std::string fFFTOptions;

    std::vector<double> fOutputReal;
    std::vector<double> fOutputComplex;
};

#endif
#endif
