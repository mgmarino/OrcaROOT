// OR1DHistoHistogramsDecoder.cc

#include <vector>
#include "OR1DHistoHistogramsDecoder.hh"
#include "ORUtils.hh"


void OR1DHistoHistogramsDecoder::Swap(UInt_t* dataRecord)
{
  if(LengthOf(dataRecord)<2) return;

  /* Swapping everything but the title. */
  ORUtils::Swap(dataRecord[1]);
  for(size_t iWord=2+NKeyWordsOf(dataRecord);iWord<LengthOf(dataRecord);iWord++) {
    ORUtils::Swap(dataRecord[iWord]);
  }
}

std::string OR1DHistoHistogramsDecoder::KeysOf(UInt_t* record) 
{ 
  std::string keys((char*)(record + 2), NKeyCharactersOf(record));
  return keys;
}


size_t OR1DHistoHistogramsDecoder::NKeysOf(UInt_t* record)
{
  size_t keyLength = NKeyCharactersOf(record);
  std::string keys = KeysOf(record);
  size_t nKeys = 1;
  for(size_t i=0; i<keyLength; i++) {
    if(keys[i] == '/') nKeys++;
  }
  return nKeys;
}

std::string OR1DHistoHistogramsDecoder::IthKeyOf(UInt_t* record, size_t iKey)
{
  size_t currentIKey=0;
  size_t iStart = 0;
  std::string keys = KeysOf(record);
  while(currentIKey<iKey) {
    if(keys[iStart] == '/') currentIKey++;
    iStart++;
  }
  size_t iStop = iStart;
  while(iStop < NKeyCharactersOf(record) && keys[iStop] != '/') iStop++;
  std::string key;
  for(size_t i=iStart; i<iStop; i++) key.push_back(keys[i]);
  return key;
}
