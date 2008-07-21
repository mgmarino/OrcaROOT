// ORCaenV260Decoder.cc

#include "TROOT.h"
#include "ORCaenV260Decoder.hh"
#include "ORLogger.hh"

std::string ORCaenV260Decoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "utime";
    case 3: return "channel";
    case 4: return "scaler";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORCaenV260Decoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return UTimeOf(record);
    case 3: return IthChannelOf(iRow);
    case 4: return IthScalerOf(record, iRow);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}

