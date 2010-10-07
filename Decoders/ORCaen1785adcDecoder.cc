// ORCaen1785adcDecoder.cc

#include "TROOT.h"
#include "ORCaen1785adcDecoder.hh"
#include "ORLogger.hh"

ORCaen1785adcDecoder::ORCaen1785adcDecoder()
{
  ORLog(kWarning) << "This decoder is derived from an untested one. If you have problems with the untested one, "
                  << "contact Jason immediately (jasondet@gmail.com). If you experience new bugs blame Jarek (kaspar@uw.edu)." << std::endl;
}

std::string ORCaen1785adcDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "channel";
    case 3: return GetValueName();
    case 4: return "underthresh";
    case 5: return "overflow";
    case 6: return "isValid";
    case 7: return "lowRange";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORCaen1785adcDecoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return IthChannelOf(record, iRow);
    case 3: return IthValueOf(record, iRow);
    case 4: return IthValueIsUnderThreshold(record, iRow);
    case 5: return IthValueIsOverflow(record, iRow);
    case 6: return IthValueIsValid(record, iRow);
    case 7: return IthValueIsLowRange(record, iRow);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}

