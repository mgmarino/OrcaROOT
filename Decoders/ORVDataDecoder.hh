// ORVDataDecoder.hh

#ifndef _ORVDataDecoder_hh_
#define _ORVDataDecoder_hh_

#include <string>
#include "TROOT.h"

class ORVDataDecoder
{
  public:
    ORVDataDecoder() {}
    virtual ~ORVDataDecoder() {}

    virtual inline bool IsShort(UInt_t* dataRecord) { return dataRecord[0] & 0x80000000; }
    virtual inline bool IsLong(UInt_t* dataRecord) { return !IsShort(dataRecord); }
    virtual inline UInt_t DataIdOf(UInt_t* dataRecord) 
      { return IsShort(dataRecord) ? dataRecord[0] & 0xfc000000 : dataRecord[0] & 0xfffc0000; }
    virtual inline UInt_t LengthOf(UInt_t* dataRecord) 
      { return IsShort(dataRecord) ? 1 : dataRecord[0] & 0x3ffff; }
    virtual void Swap(UInt_t* dataRecord);
    /* ORVDataDecoder::Swap() is called when data not matching the endianness of the host machine
     * comes in.  By default, Swap() swaps all of the long words.  If this is not needed, or
     * something different is needed, please overload it in a derived decoder. Swap does not touch
     * the first long word since this is already handled in ORVReader.   */

    virtual void DumpHex(UInt_t* dataRecord);

    static UInt_t GetIllegalDataId() { return 0x1; } // useful for error checking in ORVDataProcessor

    virtual std::string GetDataObjectPath() = 0;
};

#endif
