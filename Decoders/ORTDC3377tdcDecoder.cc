// ORTDC3377tdcDecoder.cc

#include "TROOT.h"
#include "ORTDC3377tdcDecoder.hh"
#include "ORUtils.hh"
#include "ORLogger.hh"

void ORTDC3377tdcDecoder::Swap(UInt_t* record)
{
  ORUtils::Swap(record[1]); 
  /* Swap so we can tell if this is a single or double word timestamp.*/
  size_t startSwapFrom = 3;
  if (IsDoubleWordTimestamp(record)) {
    startSwapFrom = 4;
    ORUtils::Swap(record[2]);
    ORUtils::Swap(record[3]);
    UInt_t dummy = record[2];
    record[2] = record[3];
    record[3] = dummy;
  } else {
    ORUtils::Swap(record[2]);
  }
  /* the rest of the data is 16-bit packed into 32-bits. */
  UShort_t* shortPtr = (UShort_t*) (record+startSwapFrom);; 
  for (size_t i=0;i<LengthOf(record)-startSwapFrom;i++) {
    ORUtils::Swap(shortPtr[2*i]);
    ORUtils::Swap(shortPtr[2*i+1]);
  }
}

double ORTDC3377tdcDecoder::ReferenceDateOf(UInt_t* record)
{
  double refDate = 0.0;
  if(IsDoubleWordTimestamp(record)) {
    refDate = (*((double*)(record+2)));
  }
  else  refDate = (double)(*((float*)(record + 2)));
  return refDate;
}


UInt_t ORTDC3377tdcDecoder::IthTDCValueOf(UInt_t* record, size_t i)
{
  UShort_t* data = GetTDCLocPtr(record, i);
  if (IsDoubleWordFormat(record)) {
    if ( (data[0] & 0x0100) == 0) {
      ORLog(kWarning) << "First data word of double word format does not "
                      << "have the high-word bit set: crate " << CrateOf(record)
                      << ", card " << CardOf(record) << ", channel "
                      << IthChannelOf(record, i) << ", i = " << i << std::endl;
    }
    return ((data[0] & 0x00ff) << 8) | (data[1] & 0x00ff);
  }
  else {
    if (RecordsOnLeadingEdgeOnly(record)) return (data[0] & 0x03ff);
    else return (data[0] & 0x01ff);
  }
}

std::string ORTDC3377tdcDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "channel";
    case 3: return "tdc";
    case 4: return "edge";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORTDC3377tdcDecoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return IthChannelOf(record, iRow);
    case 3: return IthTDCValueOf(record, iRow);
    case 4: return IthTDCValueIsLeadingEdge(record, iRow);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}

