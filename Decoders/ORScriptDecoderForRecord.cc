#include "ORScriptDecoderForRecord.hh"
#include "ORXmlPlist.hh"
#include "ORUtils.hh"

const ORVDictValue* ORScriptDecoderForRecord::XMLDataOf(UInt_t* record)
{
  return XMLPlistOf(record).LookUp(GetKeyOfValueInRecord()); 
}

const ORXmlPlist& ORScriptDecoderForRecord::XMLPlistOf(UInt_t* record)
{
  // We don't cache the plist, so this is called every time, for convenience.
  fXMLPlist.LoadXmlPlist((const char*)(record + kXMLData), LengthOfXMLInBytesOf(record)); 
  return fXMLPlist; 
}
void ORScriptDecoderForRecord::Swap(UInt_t* dataRecord)
{
  // Only swap the header, the rest is byte data so it 
  // doesn't need to be swapped.
  for(size_t i=1; i<kXMLData;i++) {
    ORUtils::Swap(dataRecord[i]);
  }
}
