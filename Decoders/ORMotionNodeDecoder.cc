// ORMotionNodeDecoder.cc

#include "ORMotionNodeDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
#include "ORDictionary.hh"
#include <sstream>


//**************************************************************************************

ORMotionNodeDecoder::ORMotionNodeDecoder() { fDataRecord = NULL; }

bool ORMotionNodeDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
  return true;
}

double ORMotionNodeDecoder::GetSamplingFrequency()
{
  return 100.;
}

size_t ORMotionNodeDecoder::CopyWaveformData(UInt_t* waveform, size_t len)
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLength();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLength() << std::endl;
  }
  else {
	  len = GetWaveformLength(); 
  }
  UInt_t* waveformData = GetWaveformDataPointer();
  memcpy(waveform, waveformData, sizeof(UInt_t)*len);
  return len;
}

//Error checking: **********************************************************************
bool ORMotionNodeDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << std::endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << std::endl; 
    return false;
  } 
  return true;
}

void ORMotionNodeDecoder::DumpBufferHeader()
{
  if(fDataRecord) {
    ORLog(kDebug) << "Dumping Data Buffer Header (Raw Data): " << std::endl;
    ORLog(kDebug) << "**************************************************" << std::endl;
    for(size_t i=1;i<kBufHeadLen; i++) {
      ORLog(kDebug) << fDataRecord[i] << std::endl;
    }
    ORLog(kDebug) << "**************************************************" << std::endl;
  }
}

//debugging: *************************************************************************
void ORMotionNodeDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  ORLog(kDebug) << std::endl << std::endl << "ORMotionNodeDecoder::Dump():" << std::endl ;
  if(!SetDataRecord(dataRecord)) return; 
  ORLog(kDebug) << "  Header functions: " << std::endl
    << "    Device = " << Device() << std::endl
    << "    The record is " << LengthOf(dataRecord) << " (32-bit) words long" << std::endl
    << "    The channel is " << GetChannelNum() << std::endl
    << "    Timestamp: " << GetTimeStamp() << std::endl
    << "    Record offset: " << GetRecordOffset() << std::endl
    << "    The waveform data has " << GetWaveformLength() << " (32-bit) words" << std::endl;
}

UInt_t ORMotionNodeDecoder::GetEventWaveformPoint( size_t /*event*/, size_t waveformPoint )
{
  return (UInt_t)GetWaveformDataPointer()[waveformPoint]; 
}
