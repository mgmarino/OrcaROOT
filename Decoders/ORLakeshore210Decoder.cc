// ORLakeshore210Decoder.cc

#include "ORLakeshore210Decoder.hh"
#include <iostream>
using namespace std;


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
  cout << "*************** Dumping out Lakeshore Data *************" << endl;
  cout << "Number of Channels: " << GetNumberOfChannels() << endl;
  cout << "Temperature Units: ";
  if (GetTemperatureUnits(record) == kKelvin) cout << "Kelvin" << endl;
  else cout << "Celsius" << endl;
  for (size_t i=0; i<GetNumberOfChannels();i++) {
    cout << "Channel: " << i << endl
      << " Temperature: " << GetTempOfChannel(record, i) << endl
      << " Time: " << GetTimeOfChannel(record, i) << endl;
  }
  cout << "********************************************************" << endl;
}
