// ORVBasicADCDecoder.cc

#include "TROOT.h"
#include "ORVBasicADCDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

using namespace std;

double ORVBasicADCDecoder::ReferenceDateOf(UInt_t* record)
{ 
  if(IsShort(record)) return 0.0;
  if(LengthOf(record) != 4) return 0.0;

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


string ORVBasicADCDecoder::GetHistName(int iHist)
{
  const int kNCards = 0x1f+1;
  const int kNChannels = 0xf+1;

  int iChannel = iHist % kNChannels;
  iHist = (iHist - iChannel) / kNChannels;

  int iCard = iHist % kNCards;
  int iCrate = (iHist - kNCards) / kNCards;

  return ::Form("h%s_%d_%d_%d", GetLabel().c_str(), iCrate, iCard, iChannel);
}

string ORVBasicADCDecoder::GetHistTitle(int iHist)
{
  const int kNCards = 0x1f+1;
  const int kNChannels = 0xf+1;

  int iChannel = iHist % kNChannels;
  iHist = (iHist - iChannel) / kNChannels;

  int iCard = iHist % kNCards;
  int iCrate = (iHist - kNCards) / kNCards;

  return ::Form("%s: Crate %d, Card %d, Channel %d", GetLabel().c_str(), iCrate, iCard, iChannel);
}

int ORVBasicADCDecoder::GetHistIndex(UInt_t* record)
{
  const int kNCards = 0x1f+1;
  const int kNChannels = 0xf+1;

  return int(ChannelOf(record)) + kNChannels*CardOf(record) + kNChannels*kNCards*CrateOf(record);
}

std::string ORVBasicADCDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "channel";
    case 3: return "adc";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << endl;
      return "unknown";
  }
}

UInt_t ORVBasicADCDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return ChannelOf(record);
    case 3: return ADCValueOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << endl;
      return 0;
  }
}
