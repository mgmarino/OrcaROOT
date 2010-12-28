// ORAmi286Decoder.cc

#include "ORAmi286Decoder.hh"
#include "ORLogger.hh" 

using namespace std;

//**************************************************************************************

Float_t ORAmi286Decoder::GetLevelOfChannel(UInt_t* record, UInt_t channel) 
{
  if (channel >= GetNumberOfChannels()) return 0.;
  /* Don't access out of bounds! */
  return (*((Float_t*) (record + 2*channel + 2)));
  /* packed float into a 32-bit word. */
}

UInt_t ORAmi286Decoder::GetTimeOfChannel(UInt_t* record, UInt_t channel) 
{
  if (channel >= GetNumberOfChannels()) return 0;
  /* Don't access out of bounds! */
  return (record[2*channel + 3]);
}

UShort_t ORAmi286Decoder::GetStatusOfChannel(UInt_t* record, UInt_t channel) 
{
  if (channel >= GetNumberOfChannels()) return 0;
  switch ( channel )
  {
    case 3:
      // shift to right to ensure we ship a UShort_t, 
      // rather than a ULong_t
      return (record[1] & 0xf0000000) >> 28;
    case 2:
      return (record[1] & 0xf000000) >> 24;
    case 1:
      return (record[1] & 0xf00000)>> 20;
    case 0:
      return (record[1] & 0xf0000) >> 16;
    default:
      ORLog(kWarning) << "Unknown channel " << channel << endl;
      return 0;
  }

}

void ORAmi286Decoder::Dump(UInt_t* record)
{
  ORLog(kDebug) << "*************** Dumping out Ami286 Data *************" << endl;
  ORLog(kDebug) << "Number of Channels: " << GetNumberOfChannels() << endl;
  for (size_t i=0; i<GetNumberOfChannels();i++) {
    ORLog(kDebug) << "Channel: " << i << endl
      << " Level: " << GetLevelOfChannel(record, i) << endl
      << " Time: " << GetTimeOfChannel(record, i) << endl
      << " Status: " << GetStatusOfChannel(record, i) << endl;
  }
  ORLog(kDebug) << "********************************************************" << endl;
}
