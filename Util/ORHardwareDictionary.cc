#include "ORHardwareDictionary.hh"
#include "ORDecoderDictionary.hh"
#include "ORLogger.hh"
#include <sstream> 

ORHardwareDictionary::ORHardwareDictionary(std::string name) : ORDictionary(name)
{
  /* We always look for crates/cards, but this allows us to look for other 
     dictionaries that might contain */
  fVectorOfBaseDicts.push_back("GPIB");
  fVectorOfBaseDicts.push_back("USB");
  fAppendString = " ";
} 

bool ORHardwareDictionary::LoadHardwareDictFromDict(const ORDictionary* dict)
{
  if (!dict) {
    ORLog(kError) << "Dictionary doesn't exist!" << std::endl;
    return false;
  }
  /* first let us find Crates and crate numbers*/ 
  if (!LoadCratesAndCards(dict)) return false;
  std::map<std::pair<std::string,int>,std::pair<int,std::vector<int> > >::iterator crateIter; 
  for (crateIter=fCrateMap.begin();crateIter!=fCrateMap.end();crateIter++) {
    for (size_t j=0;j<crateIter->second.second.size();j++) {
      std::ostringstream os, osEnd;
      os << crateIter->first.first << fAppendString 
         << crateIter->first.second << ":"; 
      std::string begDictStr = os.str();
      osEnd << crateIter->second.second[j];
      std::string endDictStr= osEnd.str();
      std::string middleDictStr;
      if (crateIter->second.first > 0 ) {
        middleDictStr = "station ";
      } else {
        middleDictStr = "card ";
      }
      std::string dictString = begDictStr + middleDictStr + endDictStr;
      /* Now find the dictionary*/
      const ORDictionary* bDict = dynamic_cast<const ORDictionary*>(dict->LookUp(dictString));
      if (!bDict) {
        ORLog(kError) << "Error finding: " << dictString << "in slot " 
          << crateIter->second.second[j] << std::endl;
      } 

      if (bDict) {
        const ORDictValueS* className = 
          dynamic_cast<const ORDictValueS*>(bDict->LookUp("Class Name"));
        if (!className) {
          ORLog(kError) << "Class name not found!" << std::endl;
        } else {
          /* Insert hardware object if it doesn't exist. */
          if (fDictMap.find(className->GetS())==fDictMap.end()) {
            ORLog(kError) << "Card object doesn't exist!" << std::endl;
            return false;
          }
          std::ostringstream os1, os2;
          os1 << crateIter->first.second;
          ORDictionary* classDict = dynamic_cast<ORDictionary*>(
            fDictMap[className->GetS()]);
          if (!classDict) {
            ORLog(kError) << "Class not a dictionary?" << std::endl;
            return false;
          }
          /* Insert crate if it doesn't exist. */
          if (!classDict->LookUp(os1.str())) { 
            classDict->LoadEntry(os1.str(), 
              new ORDictionary(os1.str()));
          }
          /* Finally making new dictionary from a copy and inserting. */
          ORDictionary* newCardDict = new ORDictionary(*bDict);
          /* Inserting card dictionary into crate dict.*/
          os2 << crateIter->second.second[j];
          ORDictionary* crateDict = 
            dynamic_cast<ORDictionary*>(
            classDict->LookUp(os1.str()));
          if (!crateDict) {
            ORLog(kError) << "crate dictionary not allocated correctly?" 
              << std::endl;
            return false;
          }
          newCardDict->SetName(os2.str());
          crateDict->LoadEntry(os2.str(), newCardDict);
        }
      } 
    }
  }
  return true;
}

bool ORHardwareDictionary::LoadCratesAndCards(const ORDictionary* dict)
{
  if (!dict) {
    ORLog(kError) << "Dictionary is NULL!" << std::endl;
    return false;
  }
  /* First let us grab the crate list. */
  const ORDictValueA* crateArray = 
    dynamic_cast<const ORDictValueA*>(dict->LookUp("CrateList")); 
  if (!crateArray) {
    ORLog(kError) << "Error finding CrateList" << std::endl;
    return false;
  }
  /* Now load up fCrateMap*/
  for (size_t i=0;i<crateArray->GetNValues();i++) {
    const ORDictionary* oneCrate = dynamic_cast<const ORDictionary*>(crateArray->At(i));
    if (!oneCrate) {
      ORLog(kWarning) << "CrateList array includes non-dictionary." << std::endl;
    } else {
      const ORDictValueI* crateNum = dynamic_cast<const ORDictValueI*>(
        oneCrate->LookUp("CrateNumber"));
      const ORDictValueS* crateName = dynamic_cast<const ORDictValueS*>(
        oneCrate->LookUp("ClassName"));
      const ORDictValueI* crateOffset = dynamic_cast<const ORDictValueI*>(
        oneCrate->LookUp("FirstSlot"));
      if (!crateNum || !crateName || !crateOffset) {
        ORLog(kError) << "Error finding Crate information." << std::endl;
        return false;
      } 
      /* Load up the crate map. */
      fCrateMap[std::pair<std::string, int>(crateName->GetS(), crateNum->GetI())] = 
        std::pair<int, std::vector<int> >(crateOffset->GetI(), std::vector<int>());
    }
  }

  /* Now we have the numbers of crates, we must grap the cards in the crates. */
  std::map<std::pair<std::string,int>,std::pair<int,std::vector<int> > >::iterator crateMapIter; 
  for (crateMapIter=fCrateMap.begin();crateMapIter!=fCrateMap.end();crateMapIter++) {
    /* Find the dictionary*/ 
    std::ostringstream os;
    os << crateMapIter->first.first << fAppendString 
       << crateMapIter->first.second;
    const ORDictionary* crateDict = dynamic_cast<const ORDictionary*>(dict->LookUp(os.str()));
    if (!crateDict) {
      ORLog(kError) << "Error finding dictionary: " << os.str() << std::endl; 
      return false;
    } 
    const ORDictValueA* crateDictArray = dynamic_cast<const ORDictValueA*>(
      crateDict->LookUp("CardList")); 
    if (!crateDictArray) {
      ORLog(kError) << "Error finding array CardList" << std::endl; 
      return false;
    } 
    for (size_t i=0;i<crateDictArray->GetNValues();i++) {
      const ORDictionary* cardDict = dynamic_cast<const ORDictionary*>(crateDictArray->At(i));
      if (!cardDict) {
        ORLog(kWarning) << "CardList array includes non-dictionary." << std::endl;
      } else {
        /* grab numbers, grab names*/
        const ORDictValueS* className = dynamic_cast<const ORDictValueS*>(
          cardDict->LookUp("ClassName"));
        const ORDictValueI* slotNumber = dynamic_cast<const ORDictValueI*>(
          cardDict->LookUp("SlotNumber"));
        if (!className || !slotNumber) {
          ORLog(kError) << "Error getting card." << std::endl;
          return false;
        }
        /* Is there an offset? */
        int crateNum = slotNumber->GetI() + crateMapIter->second.first;
        crateMapIter->second.second.push_back(crateNum);
        if (fDictMap.find(className->GetS()) == fDictMap.end()) {
          fDictMap[className->GetS()] = new ORDecoderDictionary(className->GetS());
        }
      }
    }
  }
  return true;
}

const ORDecoderDictionary* ORHardwareDictionary::GetDecoderDictionary(std::string dictName) const
{
  if (dictName.size() == 0) return NULL;
  const ORDecoderDictionary* dict = dynamic_cast<const ORDecoderDictionary*>(LookUp(dictName));
  if(!dict) {
    ORLog(kWarning) << "Decoder Dictionary (" << dictName << ") requested but not found" << std::endl; 
  }
  return dict; 
}
