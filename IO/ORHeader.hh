// ORHeader.hh

#ifndef _ORHeader_hh_
#define _ORHeader_hh_

#include <string>
#include "TString.h"
#include "ORXmlPlist.hh"

class ORHeader: public ORXmlPlist
{
  public:
    ORHeader(const char* fullHeaderAsString = NULL, size_t lengthOfString = 0);
    virtual ~ORHeader();

    virtual bool LoadHeaderString(const char* fullHeaderAsString, size_t lengthOfString); 
    virtual bool LoadHeaderFile(const char* fileName);
    virtual int GetDataId(std::string dataObjPath);

    virtual int GetRunNumber();

  protected:

};

#endif
