// ORBocTIC3Decoder.cc

#include "ORBocTIC3Decoder.hh"
#include "ORLogger.hh"


//**************************************************************************************

Float_t ORBocTIC3Decoder::GetPressureOfChannel(UInt_t* record, UInt_t channel) 
{
  if (channel >= GetNumberOfChannels()) return 0.;
  /* Don't access out of bounds! */
  return (*((Float_t*) (record + 2*channel + 2)));
  /* packed float into a 32-bit word. */
}

UInt_t ORBocTIC3Decoder::GetTimeOfChannel(UInt_t* record, UInt_t channel) 
{
  if (channel >= GetNumberOfChannels()) return 0;
  /* Don't access out of bounds! */
  return (record[2*channel + 3]);
}


std::string ORBocTIC3Decoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0:
      return "Device ID";
    case 1:
      return "Channel";
    case 2:
      return "Pressure";
    case 3:
      return "Time";
  }
  ORLog(kError) << "Parameter number out of bounds" << std::endl;
  return "";
}

UInt_t ORBocTIC3Decoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch (iPar) {
    case 0:
      return GetDeviceID(record);
    case 1:
      return (UInt_t)iRow;
    case 2:
      return GetPressureOfChannel(record, iRow);
    case 3:
      return GetTimeOfChannel(record, iRow);
  }
  ORLog(kError) << "Parameter number out of bounds" << std::endl;
  return 0;
}

void ORBocTIC3Decoder::Dump(UInt_t* record)
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
