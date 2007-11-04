// ORDictionary.cc

#include "ORDictionary.hh"
#include "ORLogger.hh"
#include <sstream> 

using namespace std;


ORDictionary::~ORDictionary()
{
  map<const std::string, const ORVDictValue*>::iterator i;
  for (i = fDictMap.begin(); i != fDictMap.end(); i++) {
    delete i->second;
  }
}

const ORVDictValue* ORDictionary::LookUp(string key, char delimiter) const 
{
  static std::map<const std::string, const ORVDictValue*>::const_iterator dictIter;
  size_t delimPos = key.find(delimiter); 
  if (delimPos == string::npos) {
    dictIter = fDictMap.find(key); 
    if (dictIter==fDictMap.end()) {
      ORLog(kWarning) << "ORDictionary::LookUp(): could not find key " << key
                      << " in dictionary " << fName << endl;
      return NULL;
    }
    return dictIter->second; 
  } else {
    string nextDictName = key.substr(0, delimPos);
    string nextKeyName = key.substr(delimPos+1);
    const ORVDictValue* nextDict = LookUp(nextDictName);
    if (nextDict == NULL) return NULL;
    if (nextDict->GetValueType() != kDict) {
      ORLog(kError) << "ORDictionary::LookUp(): key " << nextDictName 
                    << " in dictionary " << fName << " is not a dictionary (ValueType = " 
		    << nextDict->GetValueType() << ")." << endl;
      return NULL;
    }
    return ((ORDictionary*) nextDict)->LookUp(nextKeyName);
  }
}

std::string ORDictValueI::GetStringOfValue() const
{
  std::ostringstream o;
  if (! (o << fI)) return "ERROR";
  return o.str();
}

std::string ORDictValueR::GetStringOfValue() const
{
  std::ostringstream o;
  if (! (o << fR)) return "ERROR";
  return o.str();
}

ORDictValueA::~ORDictValueA()
{
  for (unsigned i=0; i < fDictVals.size(); i++) delete fDictVals[i];
}

