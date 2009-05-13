// ORScriptDecoderForRecord.hh

#ifndef _ORScriptDecoderForRecord_hh_
#define _ORScriptDecoderForRecord_hh_

#include "ORVDataDecoder.hh"
#include "ORXmlPlist.hh"

/*! This class deals with arbitary records that 
    are shipped in the data stream.  In particular, the 
    record format is:

    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
    ^^^^ ^^^^ ^^^^ ^^----------------------- Data ID (from header)
    -----------------^^ ^^^^ ^^^^ ^^^^ ^^^^- length in longs
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
    ^^^^------------------------------------ Script Type 1==RunScript 2==TaskScript
    -----------------------------------^^^^- Script Object ID number
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx -Unix Time (seconds from 1970)
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx -An arbitrary data tag (User specified)
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx -length of xml data in bytes
    ...
    XML data follows									
    ...

    One can access the xml data using the XMLDataOf(UInt_t*) function which
    return an ORVDictValue* pointer.  It is up to the user to determine what
    this value type is by using the value queries of the ORVDictValue class. 
    Once that is done, it may be cast as appropriate.  Another option is to
    derive from this class, test the user ID (which would indicate the value)
    and return the appopriate value in a different function.  

 */

class ORVDictValue;
class ORScriptDecoderForRecord : public ORVDataDecoder
{
  public:
    ORScriptDecoderForRecord() {}
    virtual ~ORScriptDecoderForRecord() {}

    enum EORScriptDecoderScriptType { kRunScript = 1,
                                      kTaskScript };

    enum EORScriptDecoderHeaderPosition { kScriptTypeAndID = 1,
                                          kUnixTime,
                                          kDataTag,
                                          kXMLLength,
                                          kXMLData };
    /*! Handling the correct swapping for this record. We must deal with a char buffer*/
    virtual void Swap(UInt_t* dataRecord);

    //! Overloading these functions, they don't mean anything for this record 
    virtual inline UInt_t CrateOf(UInt_t* /*record*/)
      { return 0; }
    virtual inline UInt_t ChannelOf(UInt_t* /*record*/)
      { return 0; }

    //! Header access functions for the record
    virtual inline EORScriptDecoderScriptType ScriptTypeOf(UInt_t* record) 
      { return ((EORScriptDecoderScriptType) (record[kScriptTypeAndID] >> 28)); }
    virtual inline UShort_t ScriptObjectIDOf(UInt_t* record) 
      { return (record[kScriptTypeAndID] & 0xF); } 
    virtual inline UInt_t UnixTimeOf(UInt_t* record) 
      { return record[kUnixTime]; } 
    virtual inline UInt_t UserDataTagOf(UInt_t* record) 
      { return record[kDataTag]; }
    virtual inline UInt_t LengthOfXMLInBytesOf(UInt_t* record) 
      { return record[kXMLLength]; }

    virtual const ORVDictValue* XMLDataOf(UInt_t* record); 
    virtual const ORXmlPlist& XMLPlistOf(UInt_t* record);

    virtual std::string GetKeyOfValueInRecord() { return "DataRecord"; }
    virtual std::string GetDataObjectPath() { return "ORCAScript:Record"; }

  protected:
    ORXmlPlist fXMLPlist;

};

#endif
