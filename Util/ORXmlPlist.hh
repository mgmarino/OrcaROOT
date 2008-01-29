// ORXmlPlist.hh

#ifndef _ORXmlPlist_hh_
#define _ORXmlPlist_hh_

#include <string>
#include "TString.h"
#include "ORDictionary.hh"
#include "TXMLNode.h"

class ORXmlPlist
{
  public:
    ORXmlPlist(const char* fullHeaderAsString = NULL, size_t lengthOfBuffer = 0);
    virtual ~ORXmlPlist();

    virtual bool LoadXmlPlist(const char* fullHeaderAsString, size_t lengthOfBuffer); 
    virtual bool LoadXmlPlistFromFile(const char* fileName);
    virtual ORDictionary* GetDictionary() { return fDictionary; }
    virtual const ORVDictValue* LookUp(std::string key) const;
    virtual TString& GetRawXML() { return fRawXML; }
    virtual const TString& GetRawXML() const { return fRawXML; }
    

  protected:
    virtual bool LoadDictionary(TXMLNode* dictNode, ORDictionary* dictionary); //returns true if successful
    virtual bool LoadArray(TXMLNode* dictNode, ORDictValueA* dictValueA); //returns true if successful

    virtual TXMLNode* FindChildByName(const char* name, TXMLNode* parent);

  protected:
    ORDictionary* fDictionary;
    TString fRawXML;
};

#endif
