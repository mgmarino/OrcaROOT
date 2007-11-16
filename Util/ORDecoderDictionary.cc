#include "ORDecoderDictionary.hh"
#include <sstream>

ORDecoderDictionary::ORDecoderDictionary(std::string name) : 
  ORDictionary(name)
{
}

const ORDictionary* ORDecoderDictionary::GetRecordDictWithCrateAndCard(int crate, int card) const
{
  std::ostringstream os;
  os << "crate " << crate << ":card " << card;
  return(dynamic_cast<const ORDictionary*>(LookUp(os.str()))); 
}
