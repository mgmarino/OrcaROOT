// ORBocTIC3Decoder.cc

#include "ORBocTIC3Decoder.hh"
#include <iostream>
using namespace std;


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
  cout << "*************** Dumping out BocTIC3 Data *************" << endl;
  cout << "Number of Channels: " << GetNumberOfChannels() << endl;
  for (size_t i=0; i<GetNumberOfChannels();i++) {
    cout << "Channel: " << i << endl
      << " Pressure: " << GetPressureOfChannel(record, i) << endl
      << " Time: " << GetTimeOfChannel(record, i) << endl;
  }
  cout << "********************************************************" << endl;
}
