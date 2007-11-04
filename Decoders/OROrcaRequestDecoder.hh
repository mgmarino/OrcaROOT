// OROrcaRequestDecoder.hh
/*
Decodes an xml record request that looks like the following:

*/
#ifndef _OROrcaRequestDecoder_hh_
#define _OROrcaRequestDecoder_hh_

#include "ORVDataDecoder.hh"
#include "ORXmlPlist.hh"
#include "ORDictionary.hh"
#include <string>

class OROrcaRequestDecoder: public ORVDataDecoder
{
  public:
    OROrcaRequestDecoder();
    virtual ~OROrcaRequestDecoder() {}
    enum EOROrcaRequestDecoderConsts {kBufferLength = 1};
    virtual std::string GetDataObjectPath() { return "ORCARootService:ResponsePacket"; }  
    virtual bool ParseDataRecord(UInt_t* record);
    virtual inline const UInt_t& GetRequestTag() {return fRequestTagNumber;}
    virtual inline const std::string& GetRequestType() {return fRequestType;}
    virtual inline const std::string& GetRequestOption() {return fRequestOption;}
    virtual inline const ORDictionary* GetRequestInputs() {return fRequestInputs;} 
    virtual inline UInt_t GetDataId() 
      {return (fDataRecord) ? DataIdOf(fDataRecord) : 0;} 
    virtual void Swap(UInt_t* /*dataRecord*/) {}
    /* Don't require a swap for char data. */

    //debugging:
    void Dump(UInt_t* dataRecord);
    
  protected:
    UInt_t* fDataRecord;
    std::string fRequestType;
    std::string fRequestOption;
    ORXmlPlist fXMLRecord;
    UInt_t fRequestTagNumber;
    const ORDictionary* fRequestInputs;
};

#endif
