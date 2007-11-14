// ORGretaDecoder.cc

#include "ORGretaMarkIVDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
using namespace std;

ORGretaMarkIVDecoder::ORGretaMarkIVDecoder() 
{
  fHasMasterHeader = false;
  fOffset = 0;
}

//**************************************************************************************

bool ORGretaMarkIVDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  /* first thing first, figure out if this record has a Master Header*/
  if (GetMasterHeaderLength() == kMasterHeadLen &&
      GetSlaveHeaderLength()  == kSlaveHeaderLen) {
    fHasMasterHeader = true;
    fOffset = (size_t)kMasterHeadLen+kSlaveHeaderLen;
  } else {
    fHasMasterHeader = false;
    fOffset = (size_t)kSlaveHeaderLen;
  }
  return ORGretaDecoder::SetDataRecord(dataRecord);
}



