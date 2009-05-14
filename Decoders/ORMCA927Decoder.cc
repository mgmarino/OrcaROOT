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

size_t ORMCA927Decoder::CopySpectrumData(UInt_t* spectrum, size_t len)
//copies the spectrum data to the array pointed to by
//spectrum, which is of length len
{
  size_t splen = GetSpectrumLength();
  if (splen == 0) return 0;
  if ((len < splen) || (len == 0)) {
    ORLog(kWarning) << "CopySpectrumData(): destination array length is " << len 
	  << "; spectrum data length is " << GetSpectrumLength() << std::endl;
  }
  else {
    len = GetSpectrumLength(); 
  }
  UInt_t* spectrumData = GetSpectrumDataPointer();
  memcpy(spectrum, spectrumData, sizeof(UInt_t)*len);
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
    << "    The spectrum data has " << GetSpectrumLength() << " (32-bit) words" << std::endl;
}

size_t  ORMCA927Decoder::GetSpectrumLength()		
{ 
	return (LengthOf(fDataRecord) - kBufHeadLen);
}


UInt_t ORMCA927Decoder::GetEventSpectrumPoint( size_t /*event*/, 
											   size_t spectrumPoint )
{
  return (UInt_t)GetSpectrumDataPointer()[spectrumPoint]; 
}
