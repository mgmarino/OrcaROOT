// ORGretaDecoder.cc

#include "ORGretaDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
using namespace std;


//**************************************************************************************

ORGretaDecoder::ORGretaDecoder() { fDataRecord = NULL; }
/*
void ORGretaDecoder::Swap(UInt_t* dataRecord)
{
  // Given that there is a 16-bit buffer, we have to swap things if there has 
  // been a switch of endian-ness.  This means the data looks like 2143 and it 
  // needs to look like 1234, so we just need to swap the 16-bit words. 
  // The first long word has already been swapped
  ORLog(kDebug) << "Calling Swap()" << endl; 
  UInt_t lengthOfBuffer = LengthOf(dataRecord);
  if(lengthOfBuffer < 2) return;
  UShort_t* theShortDataRecord = (UShort_t*) (dataRecord + 1);
  for(size_t i=0;i<2*(lengthOfBuffer-1);i++) {
    ORUtils::Swap(theShortDataRecord[i]);
  }
}
*/
bool ORGretaDecoder::SetDataRecord(UInt_t* dataRecord) 
//sets fDataRecord to beginning of data record; generates vector of pointers to events, 
//maps event pointers to vectors of channel pointers
{
  fDataRecord = dataRecord;

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << endl;
  if (GetPacketLength() == kBufHeadLen) {
    //ORLog(kWarning) << "SetDataRecord(): There are 0 events in the record." << endl;
    return true;
  }	
  if(!IsValid()) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << endl;
    fDataRecord = NULL;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << endl;
  return true;
}



//Channel functions: ******************************************************************


size_t ORGretaDecoder::CopyWaveformData(UShort_t* waveform, size_t len)
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
  const UInt_t* waveformData = GetWaveformDataPointer();
  // Have to use a bitmask, no memcopy possible given the packed nature of
  // the data.
  for(size_t i=0;i<len;i+=2) 
  {
    waveform[i] = waveformData[i/2] & 0xfff;  
    waveform[i+1] = (waveformData[i/2] & 0x0fff0000) >> 16;
  }
  return len;
}

size_t ORGretaDecoder::CopyWaveformDataDouble(double* waveform, size_t len)
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
  const UInt_t* waveformData = GetWaveformDataPointer();
  for(size_t i=0;i<len;i+=2) 
  {
    waveform[i] = (double) (waveformData[i/2] & 0xfff);  
    waveform[i+1] = (double) ((waveformData[i/2] & 0x0fff0000) >> 16);
  }
  return len;
}


//Error checking: **********************************************************************



bool ORGretaDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << endl; 
    return false;
  } 
  return true;
}

void ORGretaDecoder::DumpBufferHeader()
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


//debugging: *************************************************************************


void ORGretaDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  size_t iChannel, iEvent;
  size_t len; 
  UShort_t* waveform; 
  double* waveformDouble;
  
  cout << endl << endl << "ORGretaDecoder::Dump():" << endl ;
  iChannel = 0;
  iEvent = 0;
  if(!SetDataRecord(dataRecord)) return; 
	cout 
	  << "  Header functions: " << endl
	  << "    Crate = " << CrateOf() << "; card = " << CardOf() << endl
	  << "    The buffer is " << kBufHeadLen << " (32-bit) words long" << endl
	  << "    The Board Id is " << GetBoardId() << endl
	  << "    The channel is " << GetChannelNum() << endl
	  << "    The packet length is (32-bit words) " << GetPacketLength() 
            << endl
	  << "    LED/external Timestamp: " << GetLEDExtTimeStamp() << endl
	  << "    CFD Timestamp: " << GetCFDTimeStamp() << endl
	  << "    CFD Point One: " << GetCFDPointOne() << endl
	  << "    CFD Point Two: " << GetCFDPointTwo() << endl
	  << "    Energy: " << GetEnergy() << endl  
	  << "    LED Crossing is negative: " << IsLEDCrossingNeg() << endl  
	  << "    External Trigger Flag: " << IsExternalTrigFlag() << endl  
	  << "    CFD Crossing Flag: " << IsCFDCrossingFlag() << endl  
	  << "    Pileup Flag: " << IsPileupFlag() << endl  
	  << "    The waveform data has " << GetWaveformLen() 
	    << " (16-bit) words" << endl;
	  unsigned short wavedata[GetWaveformLen()];
	  waveform = wavedata;
	  double wavedataDouble[GetWaveformLen()];
	  waveformDouble = wavedataDouble;
	  len = GetWaveformLen();
	  CopyWaveformData(waveform, len);
	  CopyWaveformDataDouble(waveformDouble, len);

}
