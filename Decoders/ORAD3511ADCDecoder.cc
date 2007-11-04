// ORAD3511ADCDecoder.cc

#include "ORAD3511ADCDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

using namespace std;

double ORAD3511ADCDecoder::ReferenceDateOf(UInt_t* record)
{ 
  if(!HasReferenceDate(record)) return 0.0;

  double refDate = 0.0;
  float* rdAsFloats = (float*)(&refDate);
  if(ORUtils::MustSwap()) {
    rdAsFloats[0] = *((float*)(record + 2)) ;
    rdAsFloats[1] = *((float*)(record + 3)) ;
  } else {
    rdAsFloats[1] = *((float*)(record + 2)) ;
    rdAsFloats[0] = *((float*)(record + 3)) ;
  }
  return refDate;
}


string ORAD3511ADCDecoder::GetHistName(int iHist)
{
  const int kNCards = 0x1f+1;
  const int kNChannels = 0xf+1;

  int iChannel = iHist % kNChannels;
  iHist = (iHist - iChannel) / kNChannels;

  int iCard = iHist % kNCards;
  int iCrate = (iHist - kNCards) / kNCards;

  return ::Form("hAD3511_%d_%d_%d", iCrate, iCard, iChannel);
}

string ORAD3511ADCDecoder::GetHistTitle(int iHist)
{
  const int kNCards = 0x1f+1;

  int iCard = iHist % kNCards;
  int iCrate = (iHist - kNCards) / kNCards;

  return ::Form("AD3511: Crate %d, Card %d", iCrate, iCard);
}

int ORAD3511ADCDecoder::GetHistIndex(UInt_t* record)
{
  const int kNCards = 0x1f+1;

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
                      << ") out of range." << endl;
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
                      << ") out of range." << endl;
      return 0;
  }
}
