// ORHeader.hh

#ifndef _ORHeader_hh_
#define _ORHeader_hh_

#include <string>
#include "ORXmlPlist.hh"

//!ORHeader encapsulates an Orca Header.
class ORHeader: public ORXmlPlist
{
  public:
    ORHeader( const char* fullHeaderAsString = NULL, 
              size_t lengthOfString = 0 );
    virtual ~ORHeader();

    virtual bool LoadHeaderString( const char* fullHeaderAsString, 
                                   size_t lengthOfString ); 
    virtual bool LoadHeaderFile( const char* fileName );

    //! Returns a dataID given the object path. 
    /*!
        Orca headers contain the following construction:
        \verbatim
        <key>dataDescription</key>
        <dict>
        ...
            <key>AnObject</key>
            <dict>
            ...
                <key>DataFromObject</key>
                <dict>
                    <key>dataId</key>
                    <integer>9909</integer>
 
        ...
        \endverbatim
        To find the dataId in this case, one would call:
        GetDataId( "AnObject:DataFromObject" );

        However, the OrcaROOT framework automatically handles this and 
        this function should generally not need to be called by the user.
        
        This function is called with the return argument of 
        ORVDataDecoder::GetDataObjectPath();

        See also ORDataProcessor.
     */
    virtual int GetDataId( std::string dataObjPath ) const;

    //! Returns the run number specified in the header.
    virtual int GetRunNumber() const;

};

#endif
