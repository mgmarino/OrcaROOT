// ORXmlPlistString.cc

#include "ORXmlPlistString.hh"


ORXmlPlistString::ORXmlPlistString(size_t lengthToReserve) : std::string()
{ 
  reserve(lengthToReserve);
  Reset();
  /*Reset takes care of initializing fDictHasBeenLoaded*/
}

ORXmlPlistString::~ORXmlPlistString() 
{ 
}

void ORXmlPlistString::Reset()
{
  fDictHasBeenLoaded = false;
  clear();
  append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"><plist version=\"1.0\">\n");
}

void ORXmlPlistString::LoadDictionary(const ORDictionary* aDict)
{
  if(fDictHasBeenLoaded) Reset();
  LoadDictValue(aDict);
  ClosePlist();
  fDictHasBeenLoaded = true;
}

void ORXmlPlistString::LoadDictValue(const ORVDictValue* dictValue)
{
  if(dictValue->GetValueType() == ORVDictValue::kDict) {
    const ORDictionary* dictValuePtr = 
      dynamic_cast<const ORDictionary*>(dictValue);
    append("<dict>\n");
    const ORDictionary::DictMap& dictMap = 
      dictValuePtr->GetDictMap();
    ORDictionary::DictMap::const_iterator dictMapIter;
    for(dictMapIter=dictMap.begin();dictMapIter!=dictMap.end();dictMapIter++) {
      append("<key>");
      append(dictMapIter->first);
      append("</key>\n"); 
      LoadDictValue(dictMapIter->second);
    }
    append("</dict>\n");
  } else if(dictValue->GetValueType() == ORVDictValue::kArray) {
    append("<array>\n");
    for(size_t i=0;i<dictValue->GetNValues();i++) {
      LoadDictValue(dynamic_cast<const ORDictValueA*>(dictValue)->At(i));
    }
    append("</array>\n"); 
  } else if (dictValue->GetValueType() == ORVDictValue::kInt) {
    append("<integer>");
    append(dictValue->GetStringOfValue());
    append("</integer>\n");
  } else if (dictValue->GetValueType() == ORVDictValue::kReal) {
    append("<real>");
    append(dictValue->GetStringOfValue());
    append("</real>\n");
  } else if (dictValue->GetValueType() == ORVDictValue::kString) {
    append("<string>");
    append(dictValue->GetStringOfValue());
    append("</string>\n");
  } else if (dictValue->GetValueType() == ORVDictValue::kBool) {
    append("<");
    append(dictValue->GetStringOfValue());
    append("/>\n"); 
  } 
}
