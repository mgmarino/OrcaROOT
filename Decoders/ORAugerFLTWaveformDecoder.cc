// ORAugerFLTWaveformDecoder.cc

#include "ORAugerFLTWaveformDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
using namespace std;


//**************************************************************************************

ORAugerFLTWaveformDecoder::ORAugerFLTWaveformDecoder() { fDataRecord = NULL; }

bool ORAugerFLTWaveformDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << endl;
  if(!IsValid() || LengthOf(fDataRecord) - 2 != kBufHeadLen + kWaveformLength/2) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << endl;
    ORLog(kDebug) << "LengthOf(data record) - 2 : " << LengthOf(fDataRecord) - 2
      << " kBufHeadLen + kWaveformLength/2: " << kBufHeadLen + kWaveformLength/2 << endl;
    fDataRecord = NULL;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << endl;
  return true;
}

bool ORAugerFLTWaveformDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << endl; 
    return false;
  } 
  return true;
}

void ORAugerFLTWaveformDecoder::DumpBufferHeader()
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

size_t ORAugerFLTWaveformDecoder::CopyWaveformDataDouble(double* waveform, size_t len)
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
  const UShort_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i++) 
  {
    waveform[i] = (double) waveformData[i];  
  }
  return len;
}
//debugging: *************************************************************************


void ORAugerFLTWaveformDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  cout << endl << endl << "ORAugerFLTWaveformDecoder::Dump():" << endl ;
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
    << "    The waveform data has " << GetWaveformLen() 
      << " (32-bit) words" << endl;
}
