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

void ORBocTIC3Decoder::Dump(UInt_t* record)
{
  ORLog(kDebug) << "*************** Dumping out BocTIC3 Data *************" << std::endl;
  ORLog(kDebug) << "Number of Channels: " << GetNumberOfChannels() << std::endl;
  for (size_t i=0; i<GetNumberOfChannels();i++) {
    ORLog(kDebug) << "Channel: " << i << std::endl
      << " Pressure: " << GetPressureOfChannel(record, i) << std::endl
      << " Time: " << GetTimeOfChannel(record, i) << std::endl;
  }
  ORLog(kDebug) << "********************************************************" << std::endl;
}
