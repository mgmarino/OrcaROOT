// ORHexDumpAllProc.cc

#include "ORHexDumpAllProc.hh"
#include "ORLogger.hh"
#include "ORRunContext.hh"
#include "ORBasicDataDecoder.hh"
#include <map>
#include <string>

using namespace std;

ORHexDumpAllProc::ORHexDumpAllProc() : ORDataProcessor(NULL)
{ 
  fDataDecoder = new ORBasicDataDecoder;
}

ORHexDumpAllProc::~ORHexDumpAllProc()
{
  delete fDataDecoder;
}

ORHexDumpAllProc::EReturnCode ORHexDumpAllProc::ProcessDataRecord(UInt_t* record)
{
  if (!fDoProcess || !fDoProcessRun || !fRunContext) return kFailure;
  if(fRunContext->MustSwap() && !fRunContext->IsRecordSwapped()) {
    /* Swapping the record.  This only must be done once! */
    fDataDecoder->Swap(record);
    fRunContext->SetRecordSwapped();
  }

  static map<int, string> deviceNames = MakeIDMap();
  if(deviceNames.size() == 0) return kAlarm;
  UInt_t dataID = fDataDecoder->DataIdOf(record);
  ORLog(kRoutine) << deviceNames[dataID] << " (dataID = " << dataID << "):" << endl;
  fDataDecoder->DumpHex(record);

  return kSuccess;
}

map<int, string> ORHexDumpAllProc::MakeIDMap()
{
  map<int, string> deviceNames;
  ORDictionary* dataDescDict = (ORDictionary*) fRunContext->GetHeader()->LookUp("dataDescription");
  if(dataDescDict == NULL) {
    ORLog(kError) << "no dataDescDescription?" << endl;
    return deviceNames;
  }
  ORDictionary::DictMap dataDescMap = dataDescDict->GetDictMap();
  for(ORDictionary::DictMap::iterator i = dataDescMap.begin(); i != dataDescMap.end(); i++) {
    string deviceName = i->first;
    ORDictionary* deviceDict = (ORDictionary*) i->second;
    ORDictionary::DictMap deviceDictMap = deviceDict->GetDictMap();
    for(ORDictionary::DictMap::iterator j = deviceDictMap.begin(); j != deviceDictMap.end(); j++) {
      ORDictValueI* dataIDDVI = (ORDictValueI*) ((ORDictionary*) j->second)->LookUp("dataId");
      if(dataIDDVI == NULL) {
        ORLog(kError) << "no dataId for " << deviceName << ":" << j->first << endl;
        deviceName.clear();
        return deviceNames;
      }
      deviceNames[dataIDDVI->GetI()] = deviceName + ":" + j->first;
    }
  }
  return deviceNames;
}

