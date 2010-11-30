// ORSIS3302Decoder.cc

#include "ORSIS3302Decoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"


//**************************************************************************************

ORSIS3302Decoder::ORSIS3302Decoder() 
{ 
  fDataRecord = NULL; 
}

bool ORSIS3302Decoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
  if(!IsValid()) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << std::endl;
    fDataRecord = NULL;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
  return true;
}



//Channel functions: ******************************************************************

// Thse methods have not been changed to accomodate the extended pretrigger 

//copies the waveform data to the array pointed to by
//waveform, which is of length len
size_t ORSIS3302Decoder::CopyWaveformData(UShort_t* waveform, size_t len)
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = wflen; 
  memcpy(waveform, GetWaveformDataPointer(), len*2);
  return len;
}

//copies the waveform data to the array pointed to by
//waveform, which is of length len
size_t ORSIS3302Decoder::CopyWaveformDataDouble(double* waveform, size_t len)
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = wflen; 
  const UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i+=2) 
  {
    waveform[i] = (double) (waveformData[i/2] & 0xFFFF);  
    waveform[i+1] = (double) (waveformData[i/2] >> 16);
  }
  return len;
}


//copies the energy waveform data to the array pointed to by
//waveform, which is of length len
size_t ORSIS3302Decoder::CopyEnergyWaveformDataDouble(double* waveform, size_t len)
{
  size_t wflen = GetEnergyWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyEnergyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = wflen; 
  const UInt_t* waveformData = GetEnergyWaveformDataPointer();
  for(size_t i=0;i<len;i++) 
  {
    waveform[i] = (double)waveformData[i];  
  }
  return len;
}
/* Card/Channel settings, parameters. */

//Error checking: **********************************************************************



bool ORSIS3302Decoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << std::endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << std::endl; 
    return false;
  } 
  if(GetTrailerOffset()+GetTrailerLen() != LengthOf(fDataRecord)) {
    ORLog(kError) << "Data record lengths are inconsistent" << std::endl; 
    return false;
  }
  if(GetChannelNum() != ((fDataRecord[1] & 0xFF00) >> 8)) {
    ORLog(kError) << "Channel numbers are inconsistent" << std::endl; 
		ORLog(kError) << "((fDataRecord[1] & 0xFF00) >> 8)) = " << ((fDataRecord[1] & 0xFF00) >> 8) << std::endl; 
		ORLog(kError) << "GetChannelNum() = " << GetChannelNum() << std::endl; 

    return false;
  }
  return true;
}

void ORSIS3302Decoder::DumpBufferHeader()
{
  if(fDataRecord)
  {
    ORLog(kDebug) << "Dumping Data Buffer Header (Raw Data): " << std::endl;
    ORLog(kDebug) << "**************************************************" << std::endl;
    for(size_t i=2;i<GetBufHeadLen(); i++)
    {
      ORLog(kDebug) << fDataRecord[i] << std::endl;
    }
    ORLog(kDebug) << "**************************************************" << std::endl;
  }
}


//debugging: *************************************************************************


void ORSIS3302Decoder::Dump(UInt_t* dataRecord) //debugging 
{
  size_t iChannel, iEvent;
  
  ORLog(kDebug) << std::endl << std::endl << "ORSIS3302Decoder::Dump():" << std::endl ;
  iChannel = 0;
  iEvent = 0;
  if(!SetDataRecord(dataRecord)) return; 
	ORLog(kDebug) 
	  << "  Header functions: " << std::endl
	  << "    Crate = " << CrateOf() << "; card = " << CardOf() << std::endl
	  << "    The buffer is " << GetBufHeadLen() << " (32-bit) words long" 
      << std::endl
	  << "    The Board Id is " << GetBoardId() << std::endl
	  << "    The channel is " << GetChannelNum() << std::endl
	  << "    Timestamp: " << GetTimeStamp() << std::endl
	  << "    Energy: " << GetEnergyMax() << std::endl  
	  << "    Is Buffer Wrap Enabled: " << IsBufferWrapEnabled() << std::endl  
	  << "    The buffer starts at index (0 = not enbabled): " << GetWrapStartIndex() << std::endl
	  << "    The buffer wrap has (0 = not enbabled): " << GetNofWrapSamples() << "  samples" << std::endl
	  << "    Retrigger Flag: " << IsRetriggerFlag() << std::endl  
	  << "    ADC N+1 Flag: " << IsADCNPlusOneTriggerFlag() << std::endl  
	  << "    ADC N-1 Flag: " << IsADCNMinusOneTriggerFlag() << std::endl  
	  << "    Trigger Flag: " << IsTriggerFlag() << std::endl  
	  << "    Pileup Flag: " << IsPileupFlag() << std::endl  
	  << "    Fast Trigger Counter: " << GetFastTriggerCounter() 
      << std::endl  
	  << "    The waveform data has " << GetWaveformLen() 
	  << " (16-bit) words" << std::endl
	  << "    The energy waveform data has " << GetEnergyWaveformLen() 
	  << " (32-bit) words" << std::endl
	  << "    TRAILER (should end in 0xDEADBEEF):  " << GetTrailer() << std::endl;

}

UInt_t ORSIS3302Decoder::GetEventWaveformPoint( size_t /*event*/, 
                                                     size_t waveformPoint )
{
  if ( waveformPoint % 2 == 0 ) {
    return GetWaveformDataPointer()[waveformPoint/2] & 0xFFFF; 
  } else {
    return ( GetWaveformDataPointer()[waveformPoint/2] >> 16); 
  } 
}

