// ORXmlPlistString.hh
#ifndef _ORXmlPlistString_hh_
#define _ORXmlPlistString_hh_

#include <string>
#include "ORDictionary.hh"

/*! 
    This class handles receiving information and turning it into a plist xml
    buffer.  The buffer can be used to output or send to Orca.   
   
    Usage:
    
    \verbatim
    ORXmlPlistString aString;
    aString.LoadDictionary( someORDictionary );
    
    std::cout << aString << std::endl;  // Print out string
    ...
    aString.Reset();
    aString.LoadDictionary( someOtherORDictionary );
    \endverbatim
*/
class ORXmlPlistString : public std::string
{
  public:
    /*! 
     * Constructor.
     * Here one can pass a guess at the length, so that one can limit
     * reallocation of memory. 
     */
    ORXmlPlistString(size_t lengthToReserve = 0);
    virtual ~ORXmlPlistString();

    //!/* Load a dictionary into the string as a plist */
    virtual void LoadDictionary(const ORDictionary*);

    //!/* Sets up header, resets string. */
    virtual void Reset();

  protected:
    //!/* Closes plist bracket to make the string ready for saving or sending.*/
    virtual void ClosePlist() {append("</plist>");}

    //!/* Append a DictValue onto the string */
    virtual void LoadDictValue(const ORVDictValue*);

    bool fDictHasBeenLoaded;
};

#endif
