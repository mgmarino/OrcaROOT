#ifndef _ORDecoderDictionary_hh_
#define _ORDecoderDictionary_hh_

#ifndef _ORDictionary_hh_
#include "ORDictionary.hh"
#endif

//! Class extending ORDictionary
/*!
   Includes static information for a particular data object.
   These are generally parameters in an Orca header file
   that could be such things as configuration parameters, etc.

   For more information see ORHardwareDictionary.
 */
class ORDecoderDictionary : public ORDictionary
{
  public:
    ORDecoderDictionary(std::string name = ""); 
    virtual ~ORDecoderDictionary() {}

    //! Returns a dictionary for a record given crate and card numbers
    virtual const ORDictionary* GetRecordDictWithCrateAndCard(int crate, int card) const;
};

#endif
