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
} 

bool ORHardwareDictionary::LoadHardwareDictFromDict(const ORDictionary* dict)
{
  if (!dict) {
    ORLog(kError) << "Dictionary doesn't exist!" << std::endl;
    return false;
  }
  /* first let us find Crates and crate numbers*/ 
  if (!LoadCratesAndCards(dict)) return false;
  std::map<int, std::vector<int> >::iterator crateIter; 
  for (crateIter=fCrateMap.begin();crateIter!=fCrateMap.end();crateIter++) {
    for (size_t j=0;j<crateIter->second.size();j++) {
      std::ostringstream os;
      os << "crate " << crateIter->first << ":card " 
        << crateIter->second[j];
       const ORDictionary* bDict = dynamic_cast<const ORDictionary*>(dict->LookUp(os.str()));
      /* Now find the dictionary*/
       if (!bDict) {
         ORLog(kError) << "Error finding: " << os.str() << std::endl;
       } else {
         /* OK insert it if it doesn't exist. */
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
           os1 << "crate " << crateIter->first;
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
           os2 << "card " << crateIter->second[j];
           ORDictionary* crateDict = 
             dynamic_cast<ORDictionary*>(
             classDict->LookUp(os1.str()));
           if (!crateDict) {
             ORLog(kError) << "crate dictionary not allocated correctly?" 
               << std::endl;
             return false;
           }
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
      if (!crateNum) {
        ORLog(kError) << "Error finding CrateNumber." << std::endl;
        return false;
      } 
      /* Load up the crate map. */
      fCrateMap[crateNum->GetI()];
    }
  }

  /* Now we have the numbers of crates, we must grap the cards in the crates. */
  std::map<int, std::vector<int> >::iterator crateMapIter;
  for (crateMapIter=fCrateMap.begin();crateMapIter!=fCrateMap.end();crateMapIter++) {
    /* Find the dictionary*/ 
    std::ostringstream os;
    os << "crate " << crateMapIter->first;
    const ORDictionary* crateDict = dynamic_cast<const ORDictionary*>(dict->LookUp(os.str()));
    if (!crateDict) {
      ORLog(kError) << "Error finding dictionary: crate" << crateMapIter->first
        << std::endl;
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
        crateMapIter->second.push_back(slotNumber->GetI());
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
  return (dynamic_cast<const ORDecoderDictionary*>(LookUp(dictName)));
}
