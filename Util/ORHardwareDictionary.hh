#ifndef _ORHardwareDictionary_hh_
#define _ORHardwareDictionary_hh_

#include "ORDictionary.hh"
#include <string>
#include <vector>

class ORDecoderDictionary;
//! Hardware Dictionary class
/*!
  The Orca header includes the following construction:
  \verbatim
  <key>ObjectInfo</key>
  <dict>
          <key>Crates</key>
          <array>
                  <dict>
                          <key>Cards</key>
                          <array>
                                 <dict>
                                       <key>Card</key>
                                       <integer>0</integer>
                                       <key>Class Name</key> 
                                       <string>ORCardName</string>


  ...
  \endverbatim
  This class takes this construction and inverts it.  That is, it 
  collects all the ORCardNames into one dictionary with card/crate keys.
  To get a particular dictionary, use GetDecoderDictionary(). 
*/
class ORHardwareDictionary : public ORDictionary
{
  public:
    ORHardwareDictionary(std::string name = ""); 
    virtual ~ORHardwareDictionary() {}

    /*! 
        Finds all the hardware in the plist under:
        the keywords crate (card), etc.
        In most cases, dict will be the header dictionary parsed
        from the Orca header.
     */
    virtual bool LoadHardwareDictFromDict(const ORDictionary* dict);

    //! Given a dictName returns an ORDecoderDictionary.  Returns NULL if doesn't
    //! exist. 
    virtual const ORDecoderDictionary* GetDecoderDictionary(std::string dictName) const;

  protected:
    virtual bool LoadCratesAndCards(const ORDictionary* dict);

  protected:
   std::vector<std::string> fVectorOfBaseDicts; 
};

#endif
