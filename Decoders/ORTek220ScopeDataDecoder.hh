// ORTek220ScopeDataDecoder.hh

#ifndef _ORTek220ScopeDataDecoder_hh_
#define _ORTek220ScopeDataDecoder_hh_

#include "ORTek754DScopeDataDecoder.hh"

class ORTek220ScopeDataDecoder : public ORTek754DScopeDataDecoder
{
  public:
    ORTek220ScopeDataDecoder() {}
    virtual ~ORTek220ScopeDataDecoder() {}

    virtual std::string GetDataObjectPath() {return "ORTek220Model:ScopeData";}
};

#endif
