#ifndef _ORHardwareDictionary_hh_
#define _ORHardwareDictionary_hh_

#include "ORDictionary.hh"
#include "ORXmlPlist.hh"
#include <string>
#include <vector>
#include <map>

class ORDecoderDictionary;

class ORHardwareDictionary : public ORDictionary
{
  public:
    ORHardwareDictionary(std::string name = ""); 
    virtual ~ORHardwareDictionary() {}

    virtual bool LoadHardwareDictFromDict(const ORDictionary* dict);
      /* Finds all the hardware in the plist under:
         the keywords crate (card), etc.*/
    virtual const ORDecoderDictionary* GetDecoderDictionary(std::string dictName) const;

  protected:
    virtual bool LoadCratesAndCards(const ORDictionary* dict);

  protected:
   std::vector<std::string> fVectorOfBaseDicts; 
   std::map<int, std::vector<int> > fCrateMap;
};

#endif
