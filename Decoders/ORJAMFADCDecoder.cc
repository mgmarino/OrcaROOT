// ORJAMFADCDecoder.cc

#include "ORJAMFADCDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

void ORJAMFADCDecoder::Swap(UInt_t* record)
{
  for (size_t i=1;i<LengthOf(record);i++) {
    ORUtils::Swap(record[i]);
  }
}

std::string ORJAMFADCDecoder::GetHistName(int iHist)
{
  int iCard = iHist % kNCards;
  int iCrate = (iHist - iCard) / kNCards;

  return ::Form("hJAMF_%d_%d", iCrate, iCard);
}

std::string ORJAMFADCDecoder::GetHistTitle(int iHist)
{
  int iCard = iHist % kNCards;
  int iCrate = (iHist - iCard) / kNCards;

  return ::Form("JAMF: Crate %d, Card %d", iCrate, iCard);
}


std::string ORJAMFADCDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "channel";
    case 3: return "adc";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORJAMFADCDecoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return IthChannelOf(record, iRow);
    case 3: return IthADCValueOf(record, iRow);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}
