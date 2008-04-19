// ORXmlPlist.hh

#ifndef _ORXmlPlist_hh_
#define _ORXmlPlist_hh_

#include "TString.h"
#include "ORDictionary.hh"

/*!
   This class handles loading an xml Plist and parsing it into 
   an ORDictionary.  Plists are mac xml formats and their definitions
   can be found at: 
   
   http://www.apple.com/DTDs/PropertyList-1.0.dtd

  
 */
class TXMLNode;
class ORXmlPlist
{
  public:
    ORXmlPlist(const char* fullHeaderAsString = NULL, size_t lengthOfBuffer = 0);
    virtual ~ORXmlPlist();

    //! Load a plist from a const char* 
    virtual bool LoadXmlPlist( const char* fullHeaderAsString, 
                               size_t lengthOfBuffer ); 

    //! Load a plist from a file* 
    virtual bool LoadXmlPlistFromFile(const char* fileName);
    virtual ORDictionary* GetDictionary() { return fDictionary; }
    virtual const ORVDictValue* LookUp(std::string key) const;
    virtual TString& GetRawXML() { return fRawXML; }
    virtual const TString& GetRawXML() const { return fRawXML; }
    

  protected:
    virtual bool LoadDictionary(TXMLNode* dictNode, ORDictionary* dictionary); //<returns true if successful
    virtual bool LoadArray(TXMLNode* dictNode, ORDictValueA* dictValueA); //<returns true if successful

    virtual TXMLNode* FindChildByName(const char* name, TXMLNode* parent);

  protected:
    ORDictionary* fDictionary;
    TString fRawXML;
};

#endif
