// ORTrigger32LiveTimeDecoder.cc

#include "TROOT.h"
#include "ORTrigger32LiveTimeDecoder.hh"
#include "ORLogger.hh"

std::string ORTrigger32LiveTimeDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "trigBits";
    case 1: return "GTID";
    case 2: return "runState";
    case 3: return "crate";
    case 4: return "slot";
    case 5: return "totalLTTop";
    case 6: return "totalLTBottom";
    case 7: return "trigger1LTTop";
    case 8: return "trigger1LTBottom";
    case 9: return "trigger2LTTop";
    case 10: return "trigger2LTBottom";
    case 11: return "scopeLTTop";
    case 12: return "scopeLTBottom";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORTrigger32LiveTimeDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return EventTriggerBitsOf(record);
    case 1: return GTIDOf(record);
    case 2: return (UInt_t) RunStateOf(record);
    case 3: return CrateOf(record);
    case 4: return SlotOf(record);
    case 5: return TotalLiveTimeTopOf(record);
    case 6: return TotalLiveTimeBottomOf(record);
    case 7: return Trigger1LiveTimeTopOf(record);
    case 8: return Trigger1LiveTimeBottomOf(record);
    case 9: return Trigger2LiveTimeTopOf(record);
    case 10: return Trigger2LiveTimeBottomOf(record);
    case 11: return ScopeLiveTimeTopOf(record);
    case 12: return ScopeLiveTimeBottomOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}
