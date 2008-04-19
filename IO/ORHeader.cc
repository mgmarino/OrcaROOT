// ORHeader.cc

#include "ORHeader.hh"

#include "ORLogger.hh"
#include "ORVDataDecoder.hh"
#include "TDOMParser.h"

ORHeader::ORHeader(const char* fullHeaderAsString, size_t lengthOfString) :
  ORXmlPlist(fullHeaderAsString, lengthOfString)
{ 
}

ORHeader::~ORHeader() 
{ 
}

bool ORHeader::LoadHeaderString(const char* fullHeaderAsString, size_t lengthOfString)
{
  return LoadXmlPlist(fullHeaderAsString, lengthOfString);
}

bool ORHeader::LoadHeaderFile(const char* fileName)
{
  return LoadXmlPlistFromFile(fileName);
}


int ORHeader::GetDataId(std::string dataObjPath) const
{
  std::string fullDataObjPath = std::string("dataDescription:") + dataObjPath + ":dataId";
  ORDictValueI* dataIdDVI = (ORDictValueI*) LookUp(fullDataObjPath);
  if(dataIdDVI == NULL) {
    ORLog(kWarning) << "GetDataId(): couldn't find data id for data object path "
                    << dataObjPath << " (full header path was "
                    << fullDataObjPath << ")" << std::endl;
    return ORVDataDecoder::GetIllegalDataId();
  }
  return dataIdDVI->GetI();
}


int ORHeader::GetRunNumber() const
{ 
  ORDictValueI* runNumDictVal = (ORDictValueI*) LookUp("Run Control.RunNumber");
  if (runNumDictVal == NULL || !runNumDictVal->IsA(ORVDictValue::kInt)) {
    ORLog(kError) << "GetRunNumber(): Couldn't read run number from header; returning 0" << std::endl;
    return 0;
  }
  return runNumDictVal->GetI();
}

