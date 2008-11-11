// ORGretaDecoder.cc

#include "ORGretaMarkIVDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

ORGretaMarkIVDecoder::ORGretaMarkIVDecoder() 
{
  fHasMasterHeader = false;
  fHasSlaverHeader = false;
  fOffset = 0;
}

//**************************************************************************************

bool ORGretaMarkIVDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  /* first thing first, figure out if this record has a Master Header*/
  if (GetMasterHeaderLength() == LengthOf(dataRecord) - kORCAHeadLen) {
    /* Neither the master header exists, nor the slave header. */
    fHasMasterHeader = false;
    fHasSlaverHeader = false;
    fOffset = 0;
  } else if (GetMasterHeaderLength() == kMasterHeadLen &&
      GetSlaveHeaderLength()  == kSlaveHeaderLen) {
    fHasMasterHeader = true;
    fHasSlaverHeader = true;
    fOffset = (size_t)kMasterHeadLen+kSlaveHeaderLen;
  } else {
    fHasMasterHeader = false;
    fHasSlaverHeader = true;
    fOffset = (size_t)kSlaveHeaderLen;
  }
  return ORGretaDecoder::SetDataRecord(dataRecord);
}

UInt_t ORGretaMarkIVDecoder::GetEventWaveformPoint( size_t /*event*/, 
                                                     size_t waveformPoint )
{
  if ( waveformPoint % 2 == 0 ) {
    return (GetWaveformDataPointer()[waveformPoint/2] & fBitMask); 
  } else {
    return ( ( GetWaveformDataPointer()[waveformPoint/2] & (fBitMask << 16) )  >> 16);
  } 
}

