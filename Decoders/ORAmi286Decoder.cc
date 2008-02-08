// ORAmi286Decoder.cc

#include "ORAmi286Decoder.hh"
#include <iostream>
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

void ORAmi286Decoder::Dump(UInt_t* record)
{
  cout << "*************** Dumping out Ami286 Data *************" << endl;
  cout << "Number of Channels: " << GetNumberOfChannels() << endl;
  for (size_t i=0; i<GetNumberOfChannels();i++) {
    cout << "Channel: " << i << endl
      << " Level: " << GetLevelOfChannel(record, i) << endl
      << " Time: " << GetTimeOfChannel(record, i) << endl;
  }
  cout << "********************************************************" << endl;
}
