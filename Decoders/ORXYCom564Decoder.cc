#include "ORXYCom564Decoder.hh"
#include "ORLogger.hh"

double ORXYCom564Decoder::ReferenceDateOf(UInt_t* record)
{ 
  if(LengthOf(record) < kXYCom564HeaderSize) return 0.0;

  double refDate = record[2] + 1e-6*record[3];
  return refDate;
}

UInt_t ORXYCom564Decoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return (record[iRow + kXYCom564HeaderSize] >> 16) & 0xFF;
    case 3: { 
      UShort_t rawValue = (record[iRow + kXYCom564HeaderSize]) & 0xFFFF ;
      if (rawValue < 32768) {
          rawValue += 32768;
      } else {
          rawValue -= 32768;
      }
      return rawValue;
    }
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}
