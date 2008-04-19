// ORVTrigger32GTIDDecoder.cc

#include "TROOT.h"
#include "ORVTrigger32GTIDDecoder.hh"
#include "ORLogger.hh"

std::string ORVTrigger32GTIDDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "trigBits";
    case 1: return "GTID";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORVTrigger32GTIDDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return EventTriggerBitsOf(record);
    case 1: return GTIDOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}
