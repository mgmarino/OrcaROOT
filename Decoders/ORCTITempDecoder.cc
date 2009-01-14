// ORCTITempDecoder.cc

#include "ORCTITempDecoder.hh"
#include "ORLogger.hh"


//**************************************************************************************

void ORCTITempDecoder::Dump(UInt_t* record)
{
  ORLog(kDebug) << "*************** Dumping out CTITemp Data *************" << std::endl;
  ORLog(kDebug) << "Device ID: " << GetDeviceID(record) << std::endl
    << " Temperature: " << GetTemperature(record) << std::endl
    << " Time: " << GetTime(record) << std::endl;
  ORLog(kDebug) << "********************************************************" << std::endl;
}

std::string ORCTITempDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0:
      return "Device ID";
    case 1:
      return "Temperature";
    case 2:
      return "Time";
  }
  ORLog(kError) << "Parameter number out of bounds" << std::endl;
  return "";
}

UInt_t ORCTITempDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch (iPar) {
    case 0:
      return GetDeviceID(record);
    case 1:
      return GetTemperature(record);
    case 2:
      return GetTime(record);
  }
  ORLog(kError) << "Parameter number out of bounds" << std::endl;
  return 0;
}


