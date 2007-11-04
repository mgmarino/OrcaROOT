// ORXmlPlistString.hh
/* This class handles receiving information and turning it into a plist xml
 * buffer.  The buffer can be used to output or send to Orca.
*/
#ifndef _ORXmlPlistString_hh_
#define _ORXmlPlistString_hh_

#include <string>
#include "ORDictionary.hh"

class ORXmlPlistString : public std::string
{
  public:
    ORXmlPlistString(size_t lengthToReserve = 0);
    /* Here one can pass a guess at the length, so that one can limit
     * reallocation of memory. */
    virtual ~ORXmlPlistString();
    virtual void LoadDictionary(const ORDictionary*);
    /* Load a dictionary into the string as a plist */
    virtual void Reset();
    /* Sets up header, resets string. */
  protected:
    virtual void ClosePlist() {append("</plist>");}
    /* Closes plist bracket to make the string ready for saving or sending.*/
    virtual void LoadDictValue(const ORVDictValue*);
    /* Append a DictValue onto the string */
    bool fDictHasBeenLoaded;
};

#endif
