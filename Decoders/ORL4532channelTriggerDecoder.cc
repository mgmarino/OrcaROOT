// ORL4532channelTriggerDecoder.cc

#include "TROOT.h"
#include "ORL4532channelTriggerDecoder.hh"
#include "ORLogger.hh"

using namespace std;

string ORL4532channelTriggerDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "triggerMask";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << endl;
      return "unknown";
  }
}

UInt_t ORL4532channelTriggerDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return TriggerMaskOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << endl;
      return 0;
  }
}

