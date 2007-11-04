// ORLC950ScopeDataDecoder.hh

#ifndef _ORLC950ScopeDataDecoder_hh_
#define _ORLC950ScopeDataDecoder_hh_

#include "ORTek754DScopeDataDecoder.hh"

class ORLC950ScopeDataDecoder : public ORTek754DScopeDataDecoder 
{
  public:
    ORLC950ScopeDataDecoder() {}
    virtual ~ORLC950ScopeDataDecoder() {}

    virtual std::string GetDataObjectPath() { return "ORLC950Model:ScopeData"; }
};

#endif
