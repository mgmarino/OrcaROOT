// ORTrig4ChanDecoder.cc

#include "TROOT.h"
#include "ORTrig4ChanDecoder.hh"
#include "ORLogger.hh"


std::string ORTrig4ChanDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "trigger";
    case 1: return "UpClock";
    case 2: return "LowClock";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORTrig4ChanDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return trigOf(record);
    case 1: return UpClockOf(record);
    case 2: return LowClockOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}
