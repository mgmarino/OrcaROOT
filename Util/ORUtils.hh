// ORUtils.hh

#ifndef _ORUtils_hh_
#define _ORUtils_hh_

#ifndef ROOT_TROOT
#include "TROOT.h"
#endif

namespace ORUtils 
{
    inline bool SysIsLittleEndian()
    {
#ifndef ORBIG_ENDIAN_MACHINE
      return true;
#else
      return false; 
#endif
    }
    inline bool SysIsNotLittleEndian() { return !SysIsLittleEndian(); }
    inline UShort_t& Swap(UShort_t& x) 
      { return (x = (((x & 0xff00) >> 8) | 
                     ((x & 0x00ff) << 8))); }
    inline UInt_t& Swap(UInt_t& x) 
      {
        return (x = ((x & 0xff000000) >> 24) | 
                    ((x & 0x00ff0000) >>  8) | 
                    ((x & 0x0000ff00) <<  8) | 
                    ((x & 0x000000ff) << 24));
      }
    inline ULong64_t& Swap(ULong64_t& x) {
      return (x = ((x & 0xff00000000000000LL) >> 56) |
                  ((x & 0x00ff000000000000LL) >> 40) |
                  ((x & 0x0000ff0000000000LL) >> 24) |
                  ((x & 0x000000ff00000000LL) >>  8) |
                  ((x & 0x00000000ff000000LL) <<  8) |
                  ((x & 0x0000000000ff0000LL) << 24) |
                  ((x & 0x000000000000ff00LL) << 40) |
                  ((x & 0x00000000000000ffLL) << 56));
    }

    //Concatenates 32 bit high and low words to form a ULong64_t
    inline ULong64_t BitConcat(UInt_t lo, UInt_t hi)
    { 
      return ((ULong64_t)(lo) + (((ULong64_t)(hi)) << 32));  
    }

    //Concatenates 16 bit high, middle, and low words to form a ULong64_t
    inline ULong64_t BitConcat(UShort_t lo, UShort_t mid, UShort_t hi = 0x0)
    { 
      return ((ULong64_t)(lo) +  (((ULong64_t)(mid)) << 16) + 
               (((ULong64_t)(hi)) << 32));  
    }


};

#endif

