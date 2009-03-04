// ORGretaDecoder.cc

#include "ORGretaDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"


//**************************************************************************************

ORGretaDecoder::ORGretaDecoder() 
{ 
  fDataRecord = NULL; 
  fBitMask = 0x0;
}

bool ORGretaDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;

  if ( fBitMask == 0x0 ) {
    fBitMask = 0x1;
    for( size_t i=1;i<GetBitResolution();i++) {
      fBitMask <<= 1; 
      fBitMask += 1;
    }
  }

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
  if (GetPacketLength() == GetBufHeadLen()) {
    ORLog(kDebug) << "SetDataRecord(): There are 0 waveforms in the record." << std::endl;
    return true;
  }	
  if(!IsValid()) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << std::endl;
    fDataRecord = NULL;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
  return true;
}



//Channel functions: ******************************************************************


//copies the waveform data to the array pointed to by
//waveform, which is of length len
size_t ORGretaDecoder::CopyWaveformData(UShort_t* waveform, size_t len)
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = GetWaveformLen(); 
  const UInt_t* waveformData = GetWaveformDataPointer();
  // Have to use a bitmask, no memcopy possible given the packed nature of
  // the data.
  for(size_t i=0;i<len;i+=2) 
  {
    waveform[i] = (UShort_t) waveformData[i/2] & fBitMask;  
    waveform[i+1] = (UShort_t) (waveformData[i/2] & (fBitMask << 16) ) >> 16;
  }
  return len;
}

//copies the waveform data to the array pointed to by
//waveform, which is of length len
size_t ORGretaDecoder::CopyWaveformDataDouble(double* waveform, size_t len)
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = GetWaveformLen(); 
  const UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i+=2) 
  {
    waveform[i] = (double) (waveformData[i/2] & fBitMask);  
    waveform[i+1] = (double) ((waveformData[i/2] & (fBitMask << 16)) >> 16);
  }
  return len;
}

size_t ORGretaDecoder::CopyWaveformDataShort(short* waveform, size_t len)
{
  size_t wflen = GetWaveformLen();
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len
                    << "; waveform data length is " << GetWaveformLen() << std::endl;
  }
  else len = GetWaveformLen();
 
  memcpy(waveform, GetWaveformDataPointer(), len*sizeof(short));
  return len;
}


/* Card/Channel settings, parameters. */

UInt_t ORGretaDecoder::GetBaseAddress()
{
  return GetIntValueFromKey("baseAddress", CrateOf(), CardOf());
}

UInt_t ORGretaDecoder::GetCFDDelay()
{
  return GetIntValueFromKeyArray("CFD Delay", CrateOf(), CardOf(), GetChannelNum());
}

UInt_t ORGretaDecoder::GetCFDFraction()
{
  return GetIntValueFromKeyArray("CFD Fraction", CrateOf(), CardOf(), GetChannelNum());
}

UInt_t ORGretaDecoder::GetCFDThreshold()
{
  return GetIntValueFromKeyArray("CFD Threshold", CrateOf(), CardOf(), GetChannelNum());
}

UInt_t ORGretaDecoder::GetCollectionTime()
{
  return GetIntValueFromKey("Collection Time", CrateOf(), CardOf());
}

UInt_t ORGretaDecoder::GetDataDelay()
{
  return GetIntValueFromKeyArray("Data Delay", CrateOf(), CardOf(), GetChannelNum());
}

UInt_t ORGretaDecoder::GetDataLength()
{
  return GetIntValueFromKeyArray("Data Length", CrateOf(), CardOf(), GetChannelNum());
}

Bool_t ORGretaDecoder::IsDebugModeOn()
{
  return (GetIntValueFromKeyArray("Debug Mode", CrateOf(), CardOf(), GetChannelNum())==1);
}

Bool_t ORGretaDecoder::IsEnabled()
{
  return (GetIntValueFromKeyArray("Enabled", CrateOf(), CardOf(), GetChannelNum())==1);
}

UInt_t ORGretaDecoder::GetExternalTriggerLength()
{
  return GetIntValueFromKey("Ext Trigger Length", CrateOf(), CardOf());
}

UInt_t ORGretaDecoder::GetExternalWindow()
{
  return GetIntValueFromKey("External Window", CrateOf(), CardOf());
}

UInt_t ORGretaDecoder::GetIntegrationTime()
{
  return GetIntValueFromKey("Integration Time", CrateOf(), CardOf());
}

UInt_t ORGretaDecoder::GetLEDThreshold()
{
  return GetIntValueFromKeyArray("LED Threshold", CrateOf(), CardOf(), GetChannelNum());
}

UInt_t ORGretaDecoder::GetNoiseWindow()
{
  return GetIntValueFromKey("Noise Window", CrateOf(), CardOf());
}

Bool_t ORGretaDecoder::IsPileUpEnabled()
{
  return (GetIntValueFromKeyArray("Pile Up", CrateOf(), CardOf(), GetChannelNum())==1);
}

UInt_t ORGretaDecoder::GetPileUpWindow()
{
  return GetIntValueFromKey("Pileup Window", CrateOf(), CardOf());
}

ORGretaDecoder::EGretaPolarity ORGretaDecoder::GetPolarity()
{
  return (EGretaPolarity) GetIntValueFromKeyArray("Polarity", CrateOf(), CardOf(), GetChannelNum());
}

ORGretaDecoder::EGretaTriggerMode ORGretaDecoder::GetTriggerMode()
{
  return (EGretaTriggerMode) GetIntValueFromKeyArray("Trigger Mode", CrateOf(), CardOf(), GetChannelNum());
}

//Error checking: **********************************************************************



bool ORGretaDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << std::endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << std::endl; 
    return false;
  } 
  return true;
}

void ORGretaDecoder::DumpBufferHeader()
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


void ORGretaDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  size_t iChannel, iEvent;
  
  ORLog(kDebug) << std::endl << std::endl << "ORGretaDecoder::Dump():" << std::endl ;
  iChannel = 0;
  iEvent = 0;
  if(!SetDataRecord(dataRecord)) return; 
	ORLog(kDebug) 
	  << "  Header functions: " << std::endl
	  << "    Crate = " << CrateOf() << "; card = " << CardOf() << std::endl
	  << "    The buffer is " << GetBufHeadLen() << " (32-bit) words long" << std::endl
	  << "    The Board Id is " << GetBoardId() << std::endl
	  << "    The channel is " << GetChannelNum() << std::endl
	  << "    The packet length is (32-bit words) " << GetPacketLength() 
            << std::endl
	  << "    LED/external Timestamp: " << GetLEDExtTimeStamp() << std::endl
	  << "    CFD Timestamp: " << GetCFDTimeStamp() << std::endl
	  << "    CFD Point One: " << GetCFDPointOne() << std::endl
	  << "    CFD Point Two: " << GetCFDPointTwo() << std::endl
	  << "    Energy: " << GetEnergy() << std::endl  
	  << "    LED Crossing is negative: " << IsLEDCrossingNeg() << std::endl  
	  << "    External Trigger Flag: " << IsExternalTrigFlag() << std::endl  
	  << "    CFD Crossing Flag: " << IsCFDCrossingFlag() << std::endl  
	  << "    Pileup Flag: " << IsPileupFlag() << std::endl  
	  << "    The waveform data has " << GetWaveformLen() 
	    << " (16-bit) words" << std::endl;

}

UInt_t ORGretaDecoder::GetEventWaveformPoint( size_t /*event*/, 
                                                     size_t waveformPoint )
{
  if ( waveformPoint % 2 == 0 ) {
    return GetWaveformDataPointer()[waveformPoint/2] & fBitMask; 
  } else {
    return ( GetWaveformDataPointer()[waveformPoint/2] & (fBitMask << 16) ) >> 16; 
  } 
}

UInt_t ORGretaDecoder::GetEventFlags( size_t /*event*/ ) 
{
  return ((UInt_t)IsLEDCrossingNeg())        |
         ((UInt_t)IsExternalTrigFlag()) << 1 |
         ((UInt_t)IsCFDCrossingFlag())  << 2 |
         ((UInt_t)IsPileupFlag())       << 3;
}
