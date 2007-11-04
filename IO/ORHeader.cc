// ORHeader.cc

#include "ORHeader.hh"

#include <fstream>
#include "ORLogger.hh"
#include "ORVDataDecoder.hh"
#include "TDOMParser.h"

using namespace std;

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
  return LoadHeaderFile(fileName);
}


int ORHeader::GetDataId(string dataObjPath) 
{
  string fullDataObjPath = string("dataDescription:") + dataObjPath + ":dataId";
  ORDictValueI* dataIdDVI = (ORDictValueI*) LookUp(fullDataObjPath);
  if(dataIdDVI == NULL) {
    ORLog(kWarning) << "GetDataId(): couldn't find data id for data object path "
                    << dataObjPath << " (full header path was "
                    << fullDataObjPath << ")" << endl;
    return ORVDataDecoder::GetIllegalDataId();
  }
  return dataIdDVI->GetI();
}


int ORHeader::GetRunNumber()
{ 
  ORDictValueI* runNumDictVal = (ORDictValueI*) LookUp("Run Control.RunNumber");
  if (runNumDictVal == NULL || !runNumDictVal->IsA(ORVDictValue::kInt)) {
    ORLog(kError) << "GetRunNumber(): Couldn't read run number from header; returning 0" << endl;
    return 0;
  }
  return runNumDictVal->GetI();
}

