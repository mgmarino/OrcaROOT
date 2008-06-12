// ORAD3511ADCDecoder.cc

#include "ORAD3511ADCDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

void ORAD3511ADCDecoder::Swap(UInt_t* record)
{
  ORUtils::Swap(record[1]); 
  /* Swap so we can tell if this has a timestamp.*/
  size_t startSwapFrom = 2;
  if (HasReferenceDate(record)) {
    ORUtils::Swap(record[2]);
    ORUtils::Swap(record[3]);
    UInt_t dummy = record[2];
    record[2] = record[3];
    record[3] = dummy;
    startSwapFrom = 4;
  }
  for (size_t i=startSwapFrom;i<LengthOf(record);i++) {
    ORUtils::Swap(record[i]);
  }
}

double ORAD3511ADCDecoder::ReferenceDateOf(UInt_t* record)
{ 
  if(!HasReferenceDate(record)) return 0.0;

  double refDate = (*((double*)(record+2)));
  return refDate;
}


std::string ORAD3511ADCDecoder::GetHistName(int iHist)
{
  int iCard = iHist % kNCards;
  int iCrate = (iHist - iCard) / kNCards;

  return ::Form("hAD3511_%d_%d", iCrate, iCard);
}

std::string ORAD3511ADCDecoder::GetHistTitle(int iHist)
{
  int iCard = iHist % kNCards;
  int iCrate = (iHist - iCard) / kNCards;

  return ::Form("AD3511: Crate %d, Card %d", iCrate, iCard);
}

int ORAD3511ADCDecoder::GetHistIndex(UInt_t* record)
{
  return CardOf(record) + kNCards*CrateOf(record);
}

std::string ORAD3511ADCDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "adc";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORAD3511ADCDecoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return IthADCValueOf(record, iRow);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}
