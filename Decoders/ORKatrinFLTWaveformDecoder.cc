// ORKatrinFLTWaveformDecoder.cc

#include "ORKatrinFLTWaveformDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
using namespace std;


//**************************************************************************************

ORKatrinFLTWaveformDecoder::ORKatrinFLTWaveformDecoder() { fDataRecord = NULL; }

bool ORKatrinFLTWaveformDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << endl;
  if(!IsValid() || LengthOf(fDataRecord) != kBufHeadLen + kWaveformLength/2) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << endl;
    ORLog(kDebug) << "LengthOf(data record) : " << LengthOf(fDataRecord)
      << " kBufHeadLen + kWaveformLength/2: " << kBufHeadLen + kWaveformLength/2 << endl;
    fDataRecord = NULL;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << endl;
  return true;
}

bool ORKatrinFLTWaveformDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << endl; 
    return false;
  } 
  return true;
}

void ORKatrinFLTWaveformDecoder::DumpBufferHeader()
{
  if(fDataRecord)
  {
    ORLog(kDebug) << "Dumping Data Buffer Header (Raw Data): " << endl;
    ORLog(kDebug) << "**************************************************" << endl;
    for(size_t i=2;i<kBufHeadLen; i++)
    {
      ORLog(kDebug) << fDataRecord[i] << endl;
    }
    ORLog(kDebug) << "**************************************************" << endl;
  }
}

size_t ORKatrinFLTWaveformDecoder::CopyWaveformData( UShort_t* waveform, 
                                                     size_t len )
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << endl;
  }
  else len = GetWaveformLen(); 
  UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i+=2) 
  {
    waveform[i] = ((waveformData[i/2] & 0xFFFF0000) >> 16) ;  
    waveform[i+1] = (waveformData[i/2] & 0xFFFF);  
  }
  return len;
}

size_t ORKatrinFLTWaveformDecoder::CopyWaveformDataDouble(double* waveform, size_t len)
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << endl;
  }
  else len = GetWaveformLen(); 
  UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i+=2) 
  {
    waveform[i] = (double) ((waveformData[i/2] & 0xFFFF0000) >> 16) ;  
    waveform[i+1] = (double) (waveformData[i/2] & 0xFFFF);  
  }
  return len;
}
//debugging: *************************************************************************


void ORKatrinFLTWaveformDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  cout << endl << endl << "ORKatrinFLTWaveformDecoder::Dump():" << endl ;
  if(!SetDataRecord(dataRecord)) return; 
  cout 
    << "  Header functions: " << endl
    << "    Crate = " << CrateOf() << "; card = " << CardOf() << endl
    << "    The buffer is " << kBufHeadLen << " (32-bit) words long" << endl
    << "    The Sec is " << GetSec() << endl
    << "    The SubSec is " << GetSubSec() << endl
    << "    The channel is " << GetChannel() << endl
    << "    The channel map is " << GetChannelMap() << endl
    << "    Energy: " << GetEnergy() << endl  
    << "    The ResetSec is " << GetResetSec() << endl        // -tb- 2008-02-12
    << "    The ResetSubSec is " << GetResetSubSec() << endl  // -tb- 2008-02-12
    << "    The waveform data has " << GetWaveformLen() 
      << " (32-bit) words" << endl;
}
