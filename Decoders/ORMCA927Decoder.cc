// ORMCA927Decoder.cc

#include "ORMCA927Decoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
#include "ORDictionary.hh"
#include <sstream>


//**************************************************************************************

ORMCA927Decoder::ORMCA927Decoder() { fDataRecord = NULL; }

bool ORMCA927Decoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
  return true;
}

size_t ORMCA927Decoder::CopyWaveformData(UInt_t* waveform, size_t len)
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else {
    len = GetWaveformLen(); 
  }
  UInt_t* waveformData = GetWaveformDataPointer();
  memcpy(waveform, waveformData, sizeof(UInt_t)*len);
  return len;
}

size_t ORMCA927Decoder::CopyWaveformDataDouble(double* waveform, size_t len)
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else {
    len = GetWaveformLen(); 
  }
  UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i++) {
    waveform[i] = (Double_t) waveformData[i];  
  }
  return len;
}


//Error checking: **********************************************************************
bool ORMCA927Decoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << std::endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << std::endl; 
    return false;
  } 
  return true;
}

void ORMCA927Decoder::DumpBufferHeader()
{
  if(fDataRecord)
  {
    ORLog(kDebug) << "Dumping Data Buffer Header (Raw Data): " << std::endl;
    ORLog(kDebug) << "**************************************************" << std::endl;
    for(size_t i=1;i<kBufHeadLen; i++) {
      ORLog(kDebug) << fDataRecord[i] << std::endl;
    }
    ORLog(kDebug) << "**************************************************" << std::endl;
  }
}

//debugging: *************************************************************************
void ORMCA927Decoder::Dump(UInt_t* dataRecord) //debugging 
{
  ORLog(kDebug) << std::endl << std::endl << "ORMCA927Decoder::Dump():" << std::endl ;
  if(!SetDataRecord(dataRecord)) return; 
  ORLog(kDebug) << "  Header functions: " << std::endl
    << "    Device = " << GetDevice() << std::endl
    << "    The header is " << kBufHeadLen << " (32-bit) words long" << std::endl
    << "    The channel is " << GetChannelNum() << std::endl
    << "    The type is " << GetType() << std::endl
    << "    The zdtMode is " << GetZDTMode() << std::endl
    << "    LiveTime: " << GetLiveTime() << std::endl
    << "    RealTime: " << GetRealTime() << std::endl
    << "    The waveform data has " << GetWaveformLen() << " (32-bit) words" << std::endl;
}

UInt_t ORMCA927Decoder::GetEventWaveformPoint( size_t /*event*/, 
											   size_t waveformPoint )
{
  return (UInt_t)GetWaveformDataPointer()[waveformPoint]; 
}
