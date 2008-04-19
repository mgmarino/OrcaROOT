// ORKatrinFLTEnergyDecoder.cc

#include "TROOT.h"
#include "ORKatrinFLTEnergyDecoder.hh"
#include "ORLogger.hh"


std::string ORKatrinFLTEnergyDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "seconds";
    case 3: return "subseconds";
    case 4: return "channelMap";
    case 5: return "energy";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORKatrinFLTEnergyDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return SecondsOf(record);
    case 3: return SubSecondsOf(record);
    case 4: return ChannelMapOf(record);
    case 5: return EnergyOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}

