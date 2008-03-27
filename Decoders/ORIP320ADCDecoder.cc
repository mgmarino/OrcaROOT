// ORIP320ADCDecoder.cc

#include "ORIP320ADCDecoder.hh"
#include "ORLogger.hh"

std::string ORIP320ADCDecoder::GetParName( size_t iPar )
{
  switch(iPar) {
    case 0: return "Crate";
    case 1: return "Card";
    case 2: return "IP Slot";
    case 3: return "Time";
    case 4: return "Channel";
    case 5: return "ADC";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORIP320ADCDecoder::GetPar( UInt_t* record, 
                                  size_t iPar, 
                                  size_t iRow )
{
  switch(iPar) {
    case 0: return CrateOf( record );
    case 1: return CardOf( record );
    case 2: return IPSlotNumberOf( record );
    case 3: return TimeOf( record );
    case 4: return ChannelOf( record, iRow );
    case 5: return ADCValueOf( record, iRow );
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}
