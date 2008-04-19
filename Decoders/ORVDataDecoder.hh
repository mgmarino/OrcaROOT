// ORVDataDecoder.hh

#ifndef _ORVDataDecoder_hh_
#define _ORVDataDecoder_hh_

#include <string>
#include "TROOT.h"
#include "ORDecoderDictionary.hh"

//! Base Class for all data decoders
/** 
  ORVDataDecoder provides a number of functions for all decoders
  and additionally defines a function that must be overloaded by
  derived classes: GetDataObjectPath();
*/
class ORVDataDecoder
{

  public:
    ORVDataDecoder() {fDecoderDictionary = NULL;}
    virtual ~ORVDataDecoder() {}

    //! Is the record short, (i.e. one 32-bit word)
    virtual inline bool IsShort(UInt_t* dataRecord) 
      { return ( (dataRecord[0] & 0x80000000) == 0x80000000 ); }

    //! Is the record long
    virtual inline bool IsLong(UInt_t* dataRecord) 
      { return !IsShort(dataRecord); }

    //! Returns DataId of the record
    virtual inline UInt_t DataIdOf(UInt_t* dataRecord) 
      { return IsShort(dataRecord) ? dataRecord[0] & 0xfc000000 : 
                                     dataRecord[0] & 0xfffc0000; }

    //! Returns length of the record
    virtual inline UInt_t LengthOf(UInt_t* dataRecord) 
      { return IsShort(dataRecord) ? 1 : dataRecord[0] & 0x3ffff; }

    //! Returns crate number to which the record refers
    virtual inline UInt_t CrateOf(UInt_t* record)
      { return IsShort(record) ? (record[0] & 0x01e00000) >> 21 : 
                                 (record[1] & 0x01e00000) >> 21; }

    //! Returns card number to which the record refers
    virtual inline UInt_t CardOf(UInt_t* record)
      { return IsShort(record) ? (record[0] & 0x001f0000) >> 16 : 
                                 (record[1] & 0x001f0000) >> 16; }

    //! Handles swapping of record. 
    /**
     * ORVDataDecoder::Swap() is called when data not matching the endianness 
     * of the host machine comes in.  By default, Swap() swaps all of the long 
     * words.  If this is not needed, or something different is needed, please 
     * overload it in a derived decoder. Swap does (and should) not touch
     * the first long word since this is already handled in ORVReader.   
     */
    virtual void Swap(UInt_t* dataRecord);

    virtual void DumpHex(UInt_t* dataRecord);

    //! useful for error checking in ORVDataProcessor
    static UInt_t GetIllegalDataId() { return 0x1; } 

    //! Every decoder must overload this to return a path in the xml header.  
    /**
      For example, if the dataId is located under
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
       then GetDataObjectPath() would return "AnObject:DataFromObject".
       It automatically searches in the dataDescription dictionary and 
       automatically adds the dataId key.
    */
    virtual std::string GetDataObjectPath() = 0;

    //! This function allows us to grab a hardware dictionary from the header. 
    /**
       Some records include a hardware dictionary residing in the xml header
       that is static information associated with the hardware such as
       parameters, timing, etc.  If this function returns a non-zero sized
       string, then OrcaROOT will search for all the cards that fit this 
       parameter.
       See the description for Hardware access functions.
    */
    virtual std::string GetDictionaryObjectPath() { return ""; }
    virtual void SetDecoderDictionary(const ORDecoderDictionary* aDict) 
      { fDecoderDictionary = aDict; }

  protected:
    //! Hardware dictionary access functions.
    /**
     * Usage is as follows.  If a hardware dictionary exists and is found
     * (using the name specified in GetDictionaryObjectPath()) then
     * a Decoder may call Get[type]ValueFromKey[Array].  The key
     * refers to the name of the parameter within the dictionary.
     * For example, 
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
                                              <string>ORVmecpuModel</string>
                                      </dict>
                                      <dict>
                                              <key>Class Name>
                                              <string>ORGretina4Model</string>
                                              <key>CFD Delay</key>
                                              <array>
                                                      <integer>63</integer>
                                                      <integer>63</integer>
                                                      <integer>63</integer>
                                                      <integer>63</integer>
                                                      <integer>63</integer>
                                                      <integer>63</integer>
                                                      <integer>63</integer>
                                                      <integer>63</integer>
                                                      <integer>63</integer>
                                                      <integer>63</integer>
                                              </array>
                                              <key>CFD Fraction</key>
                                              <array>
                                                      <integer>0</integer>
                                                      <integer>0</integer>
       ...
      
       \endverbatim
     * To access the Gretina4 dictionary, GetDictionaryObjectPath() would
     * return "ORGretina4Model".  To grab the CFD Delay for index 0, 
     * one would call GetIntValueFromKeyArray("CFD Delay", CardOf(record),
     *                                         CrateOf(record), 0)
     *
     * Other access functions work similarly.
     */
    virtual const ORVDictValue* GetValueFromKey(std::string key, 
      UInt_t crate, UInt_t card);
    virtual const ORDictValueA* GetArrayFromKey(std::string key, 
      UInt_t crate, UInt_t card);

    virtual std::string GetStringValueFromKey(std::string key, 
      UInt_t crate, UInt_t card);
    virtual int GetIntValueFromKey(std::string key, 
      UInt_t crate, UInt_t card);
    virtual double GetRealValueFromKey(std::string key, 
      UInt_t crate, UInt_t card);
    virtual bool GetBoolValueFromKey(std::string key, 
      UInt_t crate, UInt_t card);
 
    virtual std::string GetStringValueFromKeyArray(std::string key, 
      UInt_t crate, UInt_t card, size_t index);
    virtual int GetIntValueFromKeyArray(std::string key, 
      UInt_t crate, UInt_t card, size_t index);
    virtual double GetRealValueFromKeyArray(std::string key, 
      UInt_t crate, UInt_t card, size_t index);
    virtual bool GetBoolValueFromKeyArray(std::string key, 
      UInt_t crate, UInt_t card, size_t index);
    

  private:
    const ORDecoderDictionary* fDecoderDictionary;
};

#endif
