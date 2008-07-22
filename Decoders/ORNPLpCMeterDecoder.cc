#include "ORNPLpCMeterDecoder.hh"
#include "ORLogger.hh"

std::string ORNPLpCMeterDecoder::GetParName( size_t iPar )
{
  switch(iPar) {
    case 0: return "Crate";
    case 1: return "Card";
    case 2: return "DeviceNumber";
    case 3: return "UnixTime";
    case 4: return "ID";
    case 5: return "IsSignal";
    case 6: return "Channel";
    case 7: return "AOrB";
    case 8: return "Data";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORNPLpCMeterDecoder::GetPar( UInt_t* record, 
                                    size_t iPar, 
                                    size_t iRow )
{
  switch(iPar) {
    case 0: return CrateOf( record );
    case 1: return CardOf( record );
    case 2: return DeviceNumberOf( record );
    case 3: return UnixTimeOf( record );
    case 4: return IDCounterOf( record, iRow );
    case 5: return IsSignalOf( record, iRow );
    case 6: return ChannelOf( record, iRow );
    case 7: return AOrBOf( record, iRow );
    case 8: return DataOf( record, iRow );
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}
