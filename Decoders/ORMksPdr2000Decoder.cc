// ORMksPdr2000Decoder.cc

#include "ORMksPdr2000Decoder.hh"
#include "ORLogger.hh"


//**************************************************************************************
//**************************************************************************************

Float_t ORMksPdr2000Decoder::GetPressureOfChannel(UInt_t* record, UInt_t channel) 
{
  if (channel >= GetNumberOfChannels()) return 0.;
  /* Don't access out of bounds! */
  return (*((Float_t*) (record + 2*channel + 2)));
  /* packed float into a 32-bit word. */
}

UInt_t ORMksPdr2000Decoder::GetTimeOfChannel(UInt_t* record, UInt_t channel) 
{
  if (channel >= GetNumberOfChannels()) return 0;
  /* Don't access out of bounds! */
  return (record[2*channel + 3]);
}


std::string ORMksPdr2000Decoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0:
      return "Device ID";
    case 1:
      return "Units";
    case 2:
      return "Channel";
    case 3:
      return "Pressure";
    case 4:
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
      return (UInt_t)iRow;
    case 3:
      return (UInt_t)GetPressureOfChannel(record, iRow);
    case 4:
      return GetTimeOfChannel(record, iRow);
  }
  ORLog(kError) << "Parameter number out of bounds" << std::endl;
  return 0;
}

void ORMksPdr2000Decoder::Dump(UInt_t* record)
{
  ORLog(kDebug) << "*************** Dumping out data *************" << std::endl;
  ORLog(kDebug) << "Number of Channels: " << GetNumberOfChannels() << std::endl;
  for (size_t i=0; i<GetNumberOfChannels();i++) {
    ORLog(kDebug) << "Channel: " << i << std::endl
      << " Pressure: " << GetPressureOfChannel(record, i) << std::endl
      << " Time: " << GetTimeOfChannel(record, i) << std::endl;
  }
  ORLog(kDebug) << "********************************************************" << std::endl;
}
