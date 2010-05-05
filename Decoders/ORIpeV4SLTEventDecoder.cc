// ORKatrinV4FLTEnergyDecoder.cc

#include "TROOT.h"
#include "ORIpeV4SLTEventDecoder.hh"
#include "ORLogger.hh"


std::string ORIpeV4SLTEventDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "CounterType";
    case 3: return "CounterSubType";
    case 4: return "EventCounter";
    case 5: return "TimestampHigh";
    case 6: return "TimestampLow";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORIpeV4SLTEventDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return CounterTypeOf(record);
    case 3: return CounterSubTypeOf(record);
    case 4: return EventCounterOf(record);
    case 5: return TimestampHighOf(record);
    case 6: return TimestampLowOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}

