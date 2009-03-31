// ORIP408DigitalIODecoder.cc

#include "ORIP408DigitalIODecoder.hh"
#include "ORLogger.hh"

std::string ORIP408DigitalIODecoder::GetParName( size_t iPar )
{
  switch(iPar) {
    case 0: return "Crate";
    case 1: return "Card";
    case 2: return "IP Slot";
    case 3: return "Time";
    case 4: return "Write Mask";
    case 5: return "Read Mask";
    case 6: return "Write Value";
    case 7: return "Read Value";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORIP408DigitalIODecoder::GetPar( UInt_t* record, 
                                  size_t iPar, 
                                  size_t iRow )
{
  switch(iPar) {
    case 0: return CrateOf( record );
    case 1: return CardOf( record );
    case 2: return IPSlotNumberOf( record );
    case 3: return TimeOf( record );
    case 4: return WriteMaskOf( record );
    case 5: return ReadMaskOf( record );
    case 6: return WriteValueOf( record );
    case 7: return ReadValueOf( record );
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}
