// ORL4532mainTriggerDecoder.cc

#include "TROOT.h"
#include "ORL4532mainTriggerDecoder.hh"
#include "ORUtils.hh"
#include "ORLogger.hh"

void ORL4532mainTriggerDecoder::Swap(UInt_t* record)
{
  ORUtils::Swap(record[1]); 
  ORUtils::Swap(record[2]); 
  /* Swap so we can tell if this has a timestamp.*/
  size_t startSwapFrom = 3;
  if (HasDoubleWordTimestamp(record)) {
    ORUtils::Swap(record[3]);
    ORUtils::Swap(record[4]);
    UInt_t dummy = record[3];
    record[3] = record[4];
    record[4] = dummy;
    startSwapFrom = 5;
  }
  for (size_t i=startSwapFrom;i<LengthOf(record);i++) {
    ORUtils::Swap(record[i]);
  }
}

double ORL4532mainTriggerDecoder::ReferenceDateOf(UInt_t* record)
{
  double refDate = 0.0;
  if(HasDoubleWordTimestamp(record)) {
    refDate = (*((double*)(record+3)));
  }
  return refDate;
}


std::string ORL4532mainTriggerDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "eventCount";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
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
                      << ") out of range." << std::endl;
      return 0;
  }
}

