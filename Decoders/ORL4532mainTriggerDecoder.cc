// ORL4532mainTriggerDecoder.cc

#include "TROOT.h"
#include "ORL4532mainTriggerDecoder.hh"
#include "ORUtils.hh"
#include "ORLogger.hh"

using namespace std;

double ORL4532mainTriggerDecoder::ReferenceDateOf(UInt_t* record)
{
  double refDate = 0.0;
  float* rdAsFloats = (float*)(&refDate);
  if(HasDoubleWordTimestamp(record)) {
    if(ORUtils::MustSwap()) {
      rdAsFloats[0] = *((float*)(record + 3)) ;
      rdAsFloats[1] = *((float*)(record + 4)) ;
    } else {
      rdAsFloats[1] = *((float*)(record + 3)) ;
      rdAsFloats[0] = *((float*)(record + 4)) ;
    }
  }
  else  refDate = double(*((float*)(record + 2)));
  return refDate;
}


string ORL4532mainTriggerDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "eventCount";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << endl;
      return "unknown";
  }
}

UInt_t ORL4532mainTriggerDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return EventCountOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << endl;
      return 0;
  }
}

