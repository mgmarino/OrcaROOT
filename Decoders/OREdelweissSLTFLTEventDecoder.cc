// OREdelweissSLTFLTEventDecoder.cc

#include "OREdelweissSLTFLTEventDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"


//**************************************************************************************

OREdelweissSLTFLTEventDecoder::OREdelweissSLTFLTEventDecoder() { fDataRecord = NULL; fWaveformLength = -1; }

bool OREdelweissSLTFLTEventDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  UInt_t eventFlags=dataRecord[7];
  // changed 2014-07-16: waveform length now is really variable -tb-
  UInt_t eventFlags4bit=eventFlags & 0xf;
  fWaveformLength = 2048;
  //if(eventFlags4bit==0x2) fWaveformLength = 2048;//this may become larger in the future -tb-
  if(eventFlags4bit>=0x2) fWaveformLength = (LengthOf(fDataRecord) - kBufHeadLen)*2;//WF is in shorts - fDataRecord is 32 bit
  //fWaveformLength = LengthOf(fDataRecord) - kBufHeadLen;


  //fWaveformLength = (  LengthOf(fDataRecord) / (kWaveformLength/2)  )  * 10000;//this sets GetWaveformLen() -tb-//TODO: check it -tb-

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
  // remarks for Till: LengthOf(...) is from ORVDataDecoder and is the length extracted from data record (in longs/int32s) -tb-
  if(!IsValid() || LengthOf(fDataRecord) != kBufHeadLen + GetWaveformLen()/2 || GetWaveformLen() > kWaveformLength) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << std::endl;
    ORLog(kDebug) << "LengthOf(data record) : " << LengthOf(fDataRecord)
      << " kBufHeadLen + GetWaveformLen()/2: " << kBufHeadLen + GetWaveformLen()/2 
      << " GetWaveformLen(): " <<  GetWaveformLen() 
      << " kWaveformLength: " << kWaveformLength << std::endl;
    fDataRecord = NULL;
    fWaveformLength = -1;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
  return true;
}

bool OREdelweissSLTFLTEventDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << std::endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << std::endl; 
    return false;
  } 
  return true;
}

void OREdelweissSLTFLTEventDecoder::DumpBufferHeader()
{
  if(fDataRecord)
  {
    ORLog(kDebug) << "Dumping Data Buffer Header (Raw Data): " << std::endl;
    ORLog(kDebug) << "**************************************************" << std::endl;
    for(size_t i=2;i<kBufHeadLen; i++)
    {
      ORLog(kDebug) << fDataRecord[i] << std::endl;
    }
    ORLog(kDebug) << "**************************************************" << std::endl;
  }
}

size_t OREdelweissSLTFLTEventDecoder::CopyWaveformData( UShort_t* waveform, 
                                                     size_t len )
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = GetWaveformLen(); 
  UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i+=2) 
  {
    //bin swapping handling changed 2008-11-18 (svn rev >=1354)  ml, -tb-
    waveform[i] = (waveformData[i/2] & 0xFFFF);  
    waveform[i+1] = ((waveformData[i/2] & 0xFFFF0000) >> 16) ;  
  }
  return len;
}

size_t OREdelweissSLTFLTEventDecoder::CopyWaveformDataDouble(double* waveform, size_t len)
//copies the waveform data to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = GetWaveformLen(); 
  UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i+=2) 
  {
    //bin swapping handling changed 2008-11-18 (svn rev >=1354)  ml, -tb-
    waveform[i] = (double) (waveformData[i/2] & 0xFFFF);  
    waveform[i+1] = (double) ((waveformData[i/2] & 0xFFFF0000) >> 16) ;  
  }
  return len;
}
//debugging: *************************************************************************


void OREdelweissSLTFLTEventDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  ORLog(kDebug) << std::endl << std::endl << "OREdelweissSLTFLTEventDecoder::Dump():" << std::endl ;
  if(!SetDataRecord(dataRecord)) return; 
  ORLog(kDebug) 
    << "  Header functions: " << std::endl
    << "    Crate = " << CrateOf() << "; card = " << CardOf() << std::endl
    << "    The buffer is " << kBufHeadLen << " (32-bit) words long" << std::endl
    << "    The Sec is " << GetSec() << std::endl
    << "    The SubSec is " << GetSubSec() << std::endl
    << "    The channel is " << GetChannel() << std::endl
    << "    The channel map is " << GetChannelMap() << std::endl
    << "    EventID: " << GetEventID() << std::endl  
    << "    Energy: " << GetEnergy() << std::endl  
    << "    EventFlags: " << GetEventFlags() << std::endl        // -tb- 2010-02-16
    << "    EventInfo: " << GetEventInfo() << std::endl  // -tb- 2010-02-16
    << "    The waveform data has " << GetWaveformLen() 
      << " (32-bit) words" << std::endl;
}
