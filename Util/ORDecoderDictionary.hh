#ifndef _ORDecoderDictionary_hh_
#define _ORDecoderDictionary_hh_

#include "ORDictionary.hh"

class ORDecoderDictionary : public ORDictionary
{
  public:
    ORDecoderDictionary(std::string name = ""); 
    virtual ~ORDecoderDictionary() {}

    virtual const ORDictionary* GetRecordDictWithCrateAndCard(int crate, int card) const;
};

#endif
