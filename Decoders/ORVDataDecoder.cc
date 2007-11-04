// ORVDataDecoder.cc

#include "ORVDataDecoder.hh"
#include "ORUtils.hh"
#include "ORLogger.hh"
#include <iomanip>

using namespace std;

void ORVDataDecoder::Swap(UInt_t* dataRecord)
{
  for(size_t i=1; i<LengthOf(dataRecord);i++) {
    ORUtils::Swap(dataRecord[i]);
  }
}

void ORVDataDecoder::DumpHex(UInt_t* dataRecord)
{
  ORLog(kRoutine) << "DumpHex for record from DataId " 
                  << DataIdOf(dataRecord) << " (= 0x" 
                  << hex << DataIdOf(dataRecord) << ")" 
                  << endl;
  for(size_t i=0; i<LengthOf(dataRecord); i++) {
    cout << i << "\t0x" << hex << setfill('0') << setw(8) << dataRecord[i] << dec << endl;
  }
}
