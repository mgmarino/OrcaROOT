// ORUtils.hh

#ifndef _ORUtils_hh_
#define _ORUtils_hh_

#include "TROOT.h"

class ORUtils 
{
  public:
    // OrcaRoot assumes by default that all Orca files are written
    // on PowerPC, in big endian. The user or a managing processor
    // (e.g. ORDataProcManager) must SetFileIsLittleEndian(bool) externally. 
    static inline bool MustSwap() { return SysIsLittleEndian() != fFileIsLittleEndian; }
    static void SetFileIsLittleEndian(bool fileType) { fFileIsLittleEndian = fileType; }
    static bool SysIsLittleEndian();

    static inline UShort_t& Swap(UShort_t& x) 
    { return (x = (((x & 0xff00) >> 8) | 
                   ((x & 0x00ff) << 8))); }
    static inline UInt_t& Swap(UInt_t& x) {
      return (x = ((x & 0xff000000) >> 24) | 
                  ((x & 0x00ff0000) >>  8) | 
                  ((x & 0x0000ff00) <<  8) | 
                  ((x & 0x000000ff) << 24));
    }
    static inline ULong64_t& Swap(ULong64_t& x) {
      return (x = ((x & 0xff00000000000000LL) >> 56) |
                  ((x & 0x00ff000000000000LL) >> 40) |
                  ((x & 0x0000ff0000000000LL) >> 24) |
                  ((x & 0x000000ff00000000LL) >>  8) |
                  ((x & 0x00000000ff000000LL) <<  8) |
                  ((x & 0x0000000000ff0000LL) << 24) |
                  ((x & 0x000000000000ff00LL) << 40) |
                  ((x & 0x00000000000000ffLL) << 56));
    }

  protected:
    static bool fFileIsLittleEndian;
};

#endif

