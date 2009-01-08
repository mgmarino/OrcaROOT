// ORMksPdr2000Decoder.cc

#include "ORMksPdr2000Decoder.hh"
#include "ORLogger.hh"


//**************************************************************************************

std::string ORMksPdr2000Decoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0:
      return "Device ID";
    case 1:
      return "Units";
    case 2:
      return "Pressure";
    case 3:
      return "Time";
  }
  ORLog(kError) << "Parameter number out of bounds" << std::endl;
  return "";
}

UInt_t ORMksPdr2000Decoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch (iPar) {
    case 0:
      return GetDeviceID(record);
    case 1:
      return GetUnits(record);
    case 2:
      return GetPressureOfChannel(record, iRow);
    case 3:
      return GetTimeOfChannel(record, iRow);
  }
  ORLog(kError) << "Parameter number out of bounds" << std::endl;
  return 0;
}

