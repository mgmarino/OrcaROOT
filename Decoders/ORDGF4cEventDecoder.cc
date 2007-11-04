// ORDGF4cEventDecoder.cc

#include "ORDGF4cEventDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
using namespace std;


//**************************************************************************************

ORDGF4cEventDecoder::ORDGF4cEventDecoder() { fDataRecord = NULL; }

void ORDGF4cEventDecoder::Swap(UInt_t* dataRecord)
{
  // Given that there is a 16-bit buffer, we have to swap things if there has 
  // been a switch of endian-ness.  This means the data looks like 2143 and it 
  // needs to look like 1234, so we just need to swap the 16-bit words. 
  // The first long word has already been swapped
  
  UInt_t lengthOfBuffer = LengthOf(dataRecord);
  if(lengthOfBuffer<=1) return;
  ORUtils::Swap(dataRecord[1]);
  UShort_t* theShortDataRecord = (UShort_t*) (dataRecord + 2);
  for(size_t i=0;i<2*(lengthOfBuffer-2);i++) {
    ORUtils::Swap(theShortDataRecord[i]);
  }
}
	
bool ORDGF4cEventDecoder::SetDataRecord(UInt_t* dataRecord) 
//sets fDataRecord to beginning of data record; generates vector of pointers to events, 
//maps event pointers to vectors of channel pointers
{
  fDataRecord = dataRecord;
  fEventPtrs.clear();
  fChannelPtrs.clear();
  fChannelNumbers.clear();
  

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << endl;
  if (GetBufNData() == kBufHeadLen) {
    //ORLog(kWarning) << "SetDataRecord(): There are 0 events in the record." << endl;
    return true;
  }	
  size_t i = 0;
  fEventPtrs.push_back(((UShort_t*) (fDataRecord + 2)) + kBufHeadLen);
  size_t length = FillChannelPtrs(i);
  for(i = 1; PtrIsInDataRecord(fEventPtrs[i-1]+length, false); i++) { 
    fEventPtrs.push_back(fEventPtrs[i-1]+length);
    length = FillChannelPtrs(i);
  }
  if(!IsValid()) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << endl;
    fEventPtrs.clear();
    fChannelPtrs.clear();
    fChannelNumbers.clear();
    fDataRecord = NULL;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << endl;
  return true;
}

size_t ORDGF4cEventDecoder::FillChannelPtrs(size_t iEvent)	
//fills vector in fChannelPtrs with the channels in iEvent; fills vector in 
//fChannelNumbers; returns length of iEvent 
{  
  UShort_t ep = fEventPtrs[iEvent][0];		//first 16 bits of event header hold # of events
  //4 LSBs determine # of channels:
  size_t num = (ep & 0x1) + ((ep & 0x2) >> 1) + ((ep & 0x4) >> 2) + ((ep & 0x8) >> 3);	
  if(num > 4) {		//there should be 0-4 channels
    ORLog(kError) << "GetNChannels(): Invalid number of channels: " << num << endl;
    return 0;
  }
  UShort_t* channelData = fEventPtrs[iEvent] + kEventHeadLen;
  size_t chanNum;
  for (size_t i = 0; i < num; i++) {
    chanNum = 0;   //number of channels
    for (size_t k = 0; k <= i;) {
      k += ((ep >> chanNum) & 0x1);
      chanNum++;
    }
    fChannelNumbers[fEventPtrs[iEvent]].push_back(chanNum - 1);
    fChannelPtrs[fEventPtrs[iEvent]].push_back(channelData);
    channelData += GetChanNData(iEvent, i);
  }
  return channelData - fEventPtrs[iEvent];
}


//Channel functions: ******************************************************************


size_t ORDGF4cEventDecoder::GetChanHeadLen()	//returns channel header length
{  
  if ( IsListModeAnyC2() ) return 4; 
  if ( IsListModeAnyC3() ) return 2; 
  if ( IsListModeAnyStdC1() ) return 9; 
  ORLog(kWarning) << "GetChanHeadLen(): Invalid channel header length" << endl; 
  return 0; 
}

UShort_t ORDGF4cEventDecoder::GetChanTrigTime(size_t iEvent, size_t iChannel)
//returns trigger time for current channel, unless another channel is specified
{ 
  if ( IsListModeAnyStdC1() ) return GetChannelPointer(iEvent, iChannel)[1]; 
  return GetChannelPointer(iEvent, iChannel)[0]; 
}

UShort_t ORDGF4cEventDecoder::GetChanEnergy(size_t iEvent, size_t iChannel)
{
  if ( IsListModeAnyStdC1() ) return ((UShort_t*)(GetChannelPointer(iEvent, iChannel)))[2]; 
  return ((UShort_t*)(GetChannelPointer(iEvent, iChannel)))[1]; 
}

double ORDGF4cEventDecoder::GetChanXiaPsa(size_t iEvent, size_t iChannel)
{
  if (IsListModeAnyStdC1()) { 
    return ((double)(GetChannelPointer(iEvent, iChannel)[3] & 0xff00 >> 8) + 
           ((double)(GetChannelPointer(iEvent, iChannel)[3] & 0xff))/256); 
  }
  if (IsListModeAnyC2()) { 
    return ((double)(GetChannelPointer(iEvent, iChannel)[2] & 0xff00 >> 8) + 
           ((double)(GetChannelPointer(iEvent, iChannel)[2] & 0xff))/256); 
  }
  ORLog(kWarning) << "GetChanXiaPsa(): Data record is in compression 3 format and "
                  << "does not contain an XIA PSA value" << endl;
  return 0xffff; 
}

UShort_t ORDGF4cEventDecoder::GetChanUserPsa(size_t iEvent, size_t iChannel)
{
  if (IsListModeAnyStdC1()) return GetChannelPointer(iEvent, iChannel)[4] & 0xff00; 
  if (IsListModeAnyC2()) return GetChannelPointer(iEvent, iChannel)[3]; 
  ORLog(kWarning) << "GetChanUserPsa(): Data record is in compression 3 format and " 
                  << "does not contain a user PSA value" << endl;
  return 0xffff; 
}

ULong64_t ORDGF4cEventDecoder::GetChanGSLT(size_t iEvent, size_t iChannel)
//returns GSLT time stamp, if available
{
  if (IsListModeAnyStdC1()) { 
    return BitConcat(GetChannelPointer(iEvent, iChannel)[7],
                     GetChannelPointer(iEvent, iChannel)[6],
                     GetChannelPointer(iEvent, iChannel)[5]); 
  }
  ORLog(kWarning) << "GetChanGSLT(): Data record is in compression 3 format and does " 
	          << "not contain a GSLT time stamp" << endl;
  return 0xffffffffffffffffLL;  
}

UShort_t ORDGF4cEventDecoder::GetChanRealTime(size_t iEvent, size_t iChannel)
//returns high word of channel real time, if available
{	  
  if (IsListModeAnyStdC1()) return GetChannelPointer(iEvent, iChannel)[8]; 
  ORLog(kWarning) << "GetChanRealTime(): Data record is in compression 3 format and " 
	          << "does not contain the real time" << endl;
  return 0xffff;  
}

size_t ORDGF4cEventDecoder::GetWaveformLen(size_t iEvent, size_t iChannel)
//returns the length of the waveform data for the current channel, unless another 
//channel is specified
{ 	   
  if (HasWaveformData()) return GetChanNData(iEvent, iChannel) - GetChanHeadLen(); 
  return 0; //in case data is in C2 or C3 format
}

size_t ORDGF4cEventDecoder::CopyWaveformData(UShort_t* waveform, size_t len, 
  size_t iEvent, size_t iChannel)
//copies the waveform data from a specified channel to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen(iEvent, iChannel);
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen(iEvent, iChannel) << endl;
  }
  else len = GetWaveformLen(iEvent, iChannel); 
  const UShort_t* waveformData = GetChannelPointer(iEvent, iChannel) + GetChanHeadLen();
  memcpy(waveform,waveformData,len*sizeof(UShort_t));
  return len;
}

size_t ORDGF4cEventDecoder::CopyWaveformDataDouble(double* waveform, size_t len,
  size_t iEvent, size_t iChannel)
//copies the waveform data from a specified channel to the array pointed to by
//waveform, which is of length len
{
  size_t wflen = GetWaveformLen(iEvent, iChannel);
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
                    << "; waveform data length is " << GetWaveformLen(iEvent, iChannel) << endl;
  }
  else len = GetWaveformLen(iEvent, iChannel); 
  const UShort_t* waveformData = GetChannelPointer(iEvent, iChannel) + GetChanHeadLen();
  for (size_t i = 0; i < len; i++) waveform[i] = (double) waveformData[i];
  return len;
}


//Error checking: **********************************************************************


bool ORDGF4cEventDecoder::PtrIsInDataRecord(UShort_t* ptr, bool verbose)
//returns true if ptr points to a valid location within the data record
{
  if (ptr == NULL) { 
    if (verbose) ORLog(kWarning) << "PtrIsInDataRecord(): pointer is NULL" << endl; 
    return false; 
  }
  if (!fEventPtrs.size()) return false; 
  if ((ptr < fEventPtrs[0]) || (ptr >= fEventPtrs[0] + GetBufNData() - kBufHeadLen)) {
    if (verbose) {
      ORLog(kWarning) << "PtrIsInDataRecord(): pointer is not within data record" 
                      << endl << "  The pointer is " << ptr - fEventPtrs[0] 
                      << " short words past the 0th event " << endl << "  The data record has " 
                      << GetBufNData() - GetChanHeadLen() << " short words past the 0th event" 
                      << endl;
    }
    return false; 
  }
  return true; 
}

bool ORDGF4cEventDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << endl; 
    return false;
  } 
  ORLog(kDebug) << "  Data record is not short" << endl;
  if (!IsListModeAnyStdC1() && !IsListModeAnyC2() && !IsListModeAnyC3() ) {
    ORLog(kError) << "The channel data is not in a valid format" << endl;
    DumpBufferHeader();
    return false;
  }
  ORLog(kDebug) << "  Channel data is in a valid format" << endl;    
  //check that BufNData matches file length XIA is in 16-bit!!!:  
  if((size_t) (2*LengthOf(fDataRecord) - 4) != (GetBufNData() + GetBufNData()%2)) {
    // 16-bit to 32-bit would create some padding for odd numbers of
    // 16-bit words, but there should always be more 32-bit words.
    ORLog(kError) << "2*Length of record - 4 (" << 2*LengthOf(fDataRecord) - 4 
                  << ") does not match length of BufNData + BufNData % 2 (" 
                  << GetBufNData() + GetBufNData()%2<< ")" << endl; 
    DumpBufferHeader();
    return false;
  } 
  ORLog(kDebug) << "  Buffer length matches file length" << endl;
  //check sum of event lengths matches buffer size:
  size_t length = kBufHeadLen;
  for (size_t i = 0; i < GetNEvents(); i++) length += GetEventLen(i);
  if(length != GetBufNData()) {
    ORLog(kError) << "The buffer is of length " << GetBufNData() 
                  << " but the sum of event lengths are of length " << length << endl; 
    DumpBufferHeader();
    return false; 
  }
  ORLog(kDebug) << "  Sum of event lengths matches buffer size" << endl;
  //check channel lengths + event header length equals event length for each event:
  for (size_t i = 0; i < GetNEvents(); i++) { 
    length = kEventHeadLen;
    for (size_t k = 0; k < GetNChannels(i); k++) length += GetChanNData(i,k);
    if (length  != GetEventLen(i)) { 
      ORLog(kError) << "Event " << i << " has length " << GetEventLen(i) 
                    << " but its header + channels are of length " << length << endl;
      DumpBufferHeader();
      return false;
    }
  } 
  ORLog(kDebug) << "  Length of event header + channel lengths matches event lengths" << endl;
  ORLog(kDebug) << "Data record is valid " << endl;
  return true;  
}

void ORDGF4cEventDecoder::DumpBufferHeader()
{
  
  ORLog(kDebug) << "Dumping Buffer Header: " << endl
    << "BufNData: " << GetBufNData() << endl
    << "BufModNum: " << GetBufModNum() << endl
    << "RunTask: " << GetRunTask() << endl
    << "BufTime: " << GetBufTime() << endl
    << "Total Events: " << GetNEvents() << endl;
  
  if(fDataRecord)
  {
    ORLog(kDebug) << "Dumping Data Record: " << endl;
    ORLog(kDebug) << "**************************************************" << endl;
    /*for(size_t i=2;i<LengthOf(fDataRecord); i+=2)
    {
      ORLog(kDebug) << fDataRecord[i] << endl;
      ORLog(kDebug) << *((UShort_t*) (fDataRecord + i)) << " "
        << *((UShort_t*) (fDataRecord + i)+ 1) << endl; 
    }*/
    ORLog(kDebug) << "**************************************************" << endl;
  }
}


//debugging: *************************************************************************


void ORDGF4cEventDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  size_t iChannel, iEvent;
  size_t len; 
  UShort_t* waveform; 
  double* waveformDouble;
  
  cout << endl << endl << "ORDGF4cEventDecoder::Dump():" << endl ;
  iChannel = 0;
  iEvent = 0;
  if(!SetDataRecord(dataRecord)) return; 
	cout 
	  << "  Header functions: " << endl
	  << "    Crate = " << CrateOf() << "; card = " << CardOf() << endl
	  << "    The buffer is " << GetBufNData() << " words long" << endl
	  << "    The module number is " << GetBufModNum() << endl
          << "    RunTask: " << GetRunTask() << " ChanHeadLen: " << GetChanHeadLen() 
	    << endl
	  << "    Run Start time: " << GetBufTime() << endl;
	  
	  cout << "  Event functions: " << endl	  
          << "    Event " << iEvent << " has " << GetNChannels(iEvent) << " channels and " 
            << GetEventLen(iEvent) << " words" << endl
	  << "    The ith active channel, where i = " << iChannel << " is channel number " 
	    << GetChannelNumber(iEvent, iChannel) << endl
	  << "    Event time: " << GetEventTime(iEvent) << endl 
	  << "    There are " << GetNEvents() << " events" << endl  
	  
	  << "  Channel functions: " << endl  
          << "    Channel " << iChannel << " in event "   
	    << iEvent << " has " << GetChanNData(iEvent, iChannel) << " words" << endl  
	  << "    Trigger time: " << GetChanTrigTime(iEvent, iChannel) << endl  
	  << "    Energy: " << GetChanEnergy(iEvent,iChannel) << endl  
	  << "    XIA PSA value: " << GetChanXiaPsa(iEvent,iChannel) << endl
	  << "    User PSA value: " << GetChanUserPsa(iEvent,iChannel) << endl
	  << "    GSLT time stamp: " << GetChanGSLT(iEvent,iChannel) << endl
	  << "    Real time: " << GetChanRealTime(iEvent,iChannel) << endl
	  << "    The waveform data has " << GetWaveformLen(iEvent,iChannel) 
	    << " words" << endl;
	  unsigned short wavedata[GetWaveformLen(iEvent,iChannel)];
	  waveform = wavedata;
	  double wavedataDouble[GetWaveformLen(iEvent,iChannel)];
	  waveformDouble = wavedataDouble;
	  len = GetWaveformLen(iEvent,iChannel);
	  CopyWaveformData(waveform, len, iEvent, iChannel);
	  CopyWaveformDataDouble(waveformDouble, len, iEvent, iChannel);

}
