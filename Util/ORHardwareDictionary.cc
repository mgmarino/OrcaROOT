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

  return true;
}

bool ORHardwareDictionary::LoadCratesAndCards(const ORDictionary* dict)
{
  /* To document, fCrateMap holds a key with a pair that is:
     pair< Name of the crate, number of the crate >

     and a value which is:
     pair< number of offset (0 or 1), vector of cards filled. */
  if (!dict) {
    ORLog(kError) << "Dictionary is NULL!" << std::endl;
    return false;
  }
  /* First let us grab the crate list. */
  const ORDictValueA* crateArray = 
    dynamic_cast<const ORDictValueA*>(dict->LookUp("ObjectInfo:Crates")); 
  if (!crateArray) {
    ORLog(kError) << "Error finding ObjectInfo:Crates" << std::endl;
    return false;
  }
  /* Now load up fCrateMap*/
  for (size_t i=0;i<crateArray->GetNValues();i++) {
    const ORDictionary* oneCrate = dynamic_cast<const ORDictionary*>(crateArray->At(i));
    if (!oneCrate) {
      ORLog(kWarning) << "Crates array includes non-dictionary." << std::endl;
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
      /* Now grab the cards from the crate. */
      const ORDictValueA* crateCardArray = dynamic_cast<const ORDictValueA*>(
        oneCrate->LookUp("Cards")); 
      if (!crateCardArray) {
        ORLog(kError) << "Error finding array Cards" << std::endl; 
        return false;
      } 
      for (size_t i=0;i<crateCardArray->GetNValues();i++) {
        const ORDictionary* cardDict = dynamic_cast<const ORDictionary*>(crateCardArray->At(i));
        if (!cardDict) {
          ORLog(kWarning) << "Cards array includes non-dictionary." << std::endl;
        } else {
          /* grab numbers, grab names*/
          const ORDictValueS* className = dynamic_cast<const ORDictValueS*>(
            cardDict->LookUp("Class Name"));
          const ORDictValueI* slotNumber = dynamic_cast<const ORDictValueI*>(
            cardDict->LookUp("Card"));
          if (!className || !slotNumber) {
            ORLog(kError) << "Error getting card." << std::endl;
            return false;
          }
          /* Is there an offset? */
          //int slotCrateNum = slotNumber->GetI() + crateOffset->GetI(); 
          int slotCrateNum = slotNumber->GetI(); 
          ORDecoderDictionary* decoderCrateDict = 0;
          
          if ((fDictMap.find(className->GetS())) == fDictMap.end()) {
            decoderCrateDict = new ORDecoderDictionary(className->GetS());
            fDictMap[className->GetS()] = decoderCrateDict; 
          } else {
            decoderCrateDict = 
              dynamic_cast<ORDecoderDictionary*>(fDictMap[className->GetS()]);
          }
          /* OK, now load it up with the card/crate info. */
          std::ostringstream os1, os2;
          os1 << crateNum->GetI();
          decoderCrateDict->LookUp(os1.str()); 
          ORDictionary* crateDictForCard = 0;

          /* Insert crate if it doesn't exist. */
          if (!(crateDictForCard = dynamic_cast<ORDictionary*>(
              decoderCrateDict->LookUp(os1.str())))) { 
            crateDictForCard = new ORDictionary(os1.str());
            decoderCrateDict->LoadEntry(os1.str(), crateDictForCard); 
          }
          /* Finally making new dictionary from a copy and inserting. */
          ORDictionary* newCardDict = new ORDictionary(*cardDict);
          os2 << slotCrateNum; 
          newCardDict->SetName(os2.str());
          crateDictForCard->LoadEntry(os2.str(), newCardDict);
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
