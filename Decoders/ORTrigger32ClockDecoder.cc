// ORTrigger32ClockDecoder.cc

#include "TROOT.h"
#include "ORTrigger32ClockDecoder.hh"
#include "ORLogger.hh"

using namespace std;

std::string ORTrigger32ClockDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "trigBits";
    case 1: return "timeTop";
    case 2: return "timeBottom";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << endl;
      return "unknown";
  }
}

UInt_t ORTrigger32ClockDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return EventTriggerBitsOf(record);
    case 1: return MacTimeTopBitsOf(record);
    case 2: return MacTimeBottomBitsOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << endl;
      return 0;
  }
}
