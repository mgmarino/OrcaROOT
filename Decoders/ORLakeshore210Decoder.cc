// ORLakeshore210Decoder.cc

#include "ORLakeshore210Decoder.hh"
#include "ORLogger.hh"


//**************************************************************************************

Float_t ORLakeshore210Decoder::GetTempOfChannel(UInt_t* record, UInt_t channel) 
{
  if (channel >= GetNumberOfChannels()) return 0.;
  /* Don't access out of bounds! */
  return (*((Float_t*) (record + 2*channel + 2)));
  /* packed float into a 32-bit word. */
}

UInt_t ORLakeshore210Decoder::GetTimeOfChannel(UInt_t* record, UInt_t channel) 
{
  if (channel >= GetNumberOfChannels()) return 0;
  /* Don't access out of bounds! */
  return (record[2*channel + 3]);
}

void ORLakeshore210Decoder::Dump(UInt_t* record)
{
  ORLog(kDebug) << "*************** Dumping out Lakeshore Data *************" << std::endl;
  ORLog(kDebug) << "Number of Channels: " << GetNumberOfChannels() << std::endl;
  ORLog(kDebug) << "Temperature Units: ";
  if (GetTemperatureUnits(record) == kKelvin) ORLog(kDebug) << "Kelvin" << std::endl;
  else ORLog(kDebug) << "Celsius" << std::endl;
  for (size_t i=0; i<GetNumberOfChannels();i++) {
    ORLog(kDebug) << "Channel: " << i << std::endl
      << " Temperature: " << GetTempOfChannel(record, i) << std::endl
      << " Time: " << GetTimeOfChannel(record, i) << std::endl;
  }
  ORLog(kDebug) << "********************************************************" << std::endl;
}

std::string ORLakeshore210Decoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0:
      return "Units";
    case 1:
      return "Channel";
    case 2:
      return "Temperature";
    case 3:
      return "Time";
  }
  ORLog(kError) << "Parameter number out of bounds" << std::endl;
  return "";
}

UInt_t ORLakeshore210Decoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch (iPar) {
    case 0:
      return (UInt_t) GetTemperatureUnits(record);
    case 1:
      return (UInt_t)iRow;
    case 2:
      return (UInt_t)GetTempOfChannel(record, iRow);
    case 3:
      return GetTimeOfChannel(record, iRow);
  }
  ORLog(kError) << "Parameter number out of bounds" << std::endl;
  return 0;
}


