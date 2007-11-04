// ORBasicDataDecoder.hh

#ifndef _ORBasicDataDecoder_hh_
#define _ORBasicDataDecoder_hh_

#include "ORVDataDecoder.hh"
#include "ORLogger.hh"

class ORBasicDataDecoder : public ORVDataDecoder
{
  public:
    virtual std::string GetDataObjectPath() { 
      ORLog(kWarning) << "GetDataObjectPath() should not be called for an ORBasicDataDecoder! " 
                      << "returning an empty string." << std::endl;
      return ""; 
    }
};

#endif
