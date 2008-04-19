// ORDGF4cEventDecoder.cc

#include "ORDGF4cEventDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"


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
  fEventVector.clear();
  

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
  if (GetBufNData() == kBufHeadLen) {
    //ORLog(kWarning) << "SetDataRecord(): There are 0 events in the record." << std::endl;
    return true;
  }	
  size_t i = 0;
  fEventPtrs.push_back(((UShort_t*) (fDataRecord + 2)) + kBufHeadLen);
  size_t length = FillChannelPtrs(i);
  for (size_t j=0;j<GetNChannels(i);j++) {
    fEventVector.push_back(std::pair<size_t, size_t>(i, j));
  }
  for(i = 1; PtrIsInDataRecord(fEventPtrs[i-1]+length, false); i++) { 
    fEventPtrs.push_back(fEventPtrs[i-1]+length);
    length = FillChannelPtrs(i);
    for (size_t j=0;j<GetNChannels(i);j++) {
      fEventVector.push_back(std::pair<size_t, size_t>(i, j));
    }
  }
  if(!IsValid()) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << std::endl;
    fEventPtrs.clear();
    fChannelPtrs.clear();
    fChannelNumbers.clear();
    fDataRecord = NULL;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
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
    ORLog(kError) << "GetNChannels(): Invalid number of channels: " << num << std::endl;
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
  ORLog(kWarning) << "GetChanHeadLen(): Invalid channel header length" << std::endl; 
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
                  << "does not contain an XIA PSA value" << std::endl;
  return 0xffff; 
}

UShort_t ORDGF4cEventDecoder::GetChanUserPsa(size_t iEvent, size_t iChannel)
{
  if (IsListModeAnyStdC1()) return GetChannelPointer(iEvent, iChannel)[4] & 0xff00; 
  if (IsListModeAnyC2()) return GetChannelPointer(iEvent, iChannel)[3]; 
  ORLog(kWarning) << "GetChanUserPsa(): Data record is in compression 3 format and " 
                  << "does not contain a user PSA value" << std::endl;
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
	          << "not contain a GSLT time stamp" << std::endl;
  return 0xffffffffffffffffLL;  
}

UShort_t ORDGF4cEventDecoder::GetChanRealTime(size_t iEvent, size_t iChannel)
//returns high word of channel real time, if available
{	  
  if (IsListModeAnyStdC1()) return GetChannelPointer(iEvent, iChannel)[8]; 
  ORLog(kWarning) << "GetChanRealTime(): Data record is in compression 3 format and " 
	          << "does not contain the real time" << std::endl;
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
                    << "; waveform data length is " << GetWaveformLen(iEvent, iChannel) << std::endl;
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
                    << "; waveform data length is " << GetWaveformLen(iEvent, iChannel) << std::endl;
  }
  else len = GetWaveformLen(iEvent, iChannel); 
  const UShort_t* waveformData = GetChannelPointer(iEvent, iChannel) + GetChanHeadLen();
  for (size_t i = 0; i < len; i++) waveform[i] = (double) waveformData[i];
  return len;
}

/* Card/Channel settings, parameters. */

UInt_t ORDGF4cEventDecoder::GetBinFactor(size_t channel)
{
  return GetIntValueFromKeyArray("binFactor", CrateOf(), CardOf(), channel);
}

UInt_t ORDGF4cEventDecoder::GetCutoffEMin(size_t channel)
{
  return GetIntValueFromKeyArray("cutoffEMin", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetEnergyFlatTop(size_t channel)
{
  return GetRealValueFromKeyArray("energyFlatTop", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetEnergyRiseTime(size_t channel)
{
  return GetRealValueFromKeyArray("energyRiseTime", CrateOf(), CardOf(), channel);
}

Bool_t ORDGF4cEventDecoder::IsInSync()
{
  return GetBoolValueFromKey("inSync", CrateOf(), CardOf());
}

double ORDGF4cEventDecoder::GetPSAEnd(size_t channel)
{
  return GetRealValueFromKeyArray("psaEnd", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetPSAStart(size_t channel)
{
  return GetRealValueFromKeyArray("psaStart", CrateOf(), CardOf(), channel);
}

UInt_t ORDGF4cEventDecoder::GetRunBehavior()
{
  return GetIntValueFromKey("runBehavior", CrateOf(), CardOf());
}

Bool_t ORDGF4cEventDecoder::IsSyncWait()
{
  return GetBoolValueFromKey("syncWait", CrateOf(), CardOf());
}

double ORDGF4cEventDecoder::GetTau(size_t channel)
{
  return GetRealValueFromKeyArray("tau", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetTauSigma(size_t channel)
{
  return GetRealValueFromKeyArray("tauSigma", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetTraceDelay(size_t channel)
{
  return GetRealValueFromKeyArray("traceDelay", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetTraceLength(size_t channel)
{
  return GetRealValueFromKeyArray("traceLength", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetTriggerFlatTop(size_t channel)
{
  return GetRealValueFromKeyArray("triggerFlatTop", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetTriggerRiseTime(size_t channel)
{
  return GetRealValueFromKeyArray("triggerRiseTime", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetTriggerThreshold(size_t channel)
{
  return GetRealValueFromKeyArray("triggerThreshold", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetVGain(size_t channel)
{
  return GetRealValueFromKeyArray("vGain", CrateOf(), CardOf(), channel);
}

double ORDGF4cEventDecoder::GetVOffset(size_t channel)
{
  return GetRealValueFromKeyArray("vOffset", CrateOf(), CardOf(), channel);
}

UInt_t ORDGF4cEventDecoder::GetXWait(size_t channel)
{
  return GetIntValueFromKeyArray("xWait", CrateOf(), CardOf(), channel);
}



//Error checking: **********************************************************************


bool ORDGF4cEventDecoder::PtrIsInDataRecord(UShort_t* ptr, bool verbose)
//returns true if ptr points to a valid location within the data record
{
  if (ptr == NULL) { 
    if (verbose) ORLog(kWarning) << "PtrIsInDataRecord(): pointer is NULL" << std::endl; 
    return false; 
  }
  if (!fEventPtrs.size()) return false; 
  if ((ptr < fEventPtrs[0]) || (ptr >= fEventPtrs[0] + GetBufNData() - kBufHeadLen)) {
    if (verbose) {
      ORLog(kWarning) << "PtrIsInDataRecord(): pointer is not within data record" 
                      << std::endl << "  The pointer is " << ptr - fEventPtrs[0] 
                      << " short words past the 0th event " << std::endl << "  The data record has " 
                      << GetBufNData() - GetChanHeadLen() << " short words past the 0th event" 
                      << std::endl;
    }
    return false; 
  }
  return true; 
}

bool ORDGF4cEventDecoder::IsValid() 
{ 
  ORLog(kDebug) << "IsValid(): starting... " << std::endl;
  if(IsShort(fDataRecord)) { 
    ORLog(kError) << "Data file is short" << std::endl; 
    return false;
  } 
  ORLog(kDebug) << "  Data record is not short" << std::endl;
  if (!IsListModeAnyStdC1() && !IsListModeAnyC2() && !IsListModeAnyC3() ) {
    ORLog(kError) << "The channel data is not in a valid format" << std::endl;
    DumpBufferHeader();
    return false;
  }
  ORLog(kDebug) << "  Channel data is in a valid format" << std::endl;    
  //check that BufNData matches file length XIA is in 16-bit!!!:  
  if((size_t) (2*LengthOf(fDataRecord) - 4) != (GetBufNData() + GetBufNData()%2)) {
    // 16-bit to 32-bit would create some padding for odd numbers of
    // 16-bit words, but there should always be more 32-bit words.
    ORLog(kError) << "2*Length of record - 4 (" << 2*LengthOf(fDataRecord) - 4 
                  << ") does not match length of BufNData + BufNData % 2 (" 
                  << GetBufNData() + GetBufNData()%2<< ")" << std::endl; 
    DumpBufferHeader();
    return false;
  } 
  ORLog(kDebug) << "  Buffer length matches file length" << std::endl;
  //check sum of event lengths matches buffer size:
  size_t length = kBufHeadLen;
  for (size_t i = 0; i < GetDGFNEvents(); i++) length += GetDGFEventLen(i);
  if(length != GetBufNData()) {
    ORLog(kError) << "The buffer is of length " << GetBufNData() 
                  << " but the sum of event lengths are of length " << length << std::endl; 
    DumpBufferHeader();
    return false; 
  }
  ORLog(kDebug) << "  Sum of event lengths matches buffer size" << std::endl;
  //check channel lengths + event header length equals event length for each event:
  for (size_t i = 0; i < GetDGFNEvents(); i++) { 
    length = kEventHeadLen;
    for (size_t k = 0; k < GetNChannels(i); k++) length += GetChanNData(i,k);
    if (length  != GetDGFEventLen(i)) { 
      ORLog(kError) << "Event " << i << " has length " << GetDGFEventLen(i) 
                    << " but its header + channels are of length " << length << std::endl;
      DumpBufferHeader();
      return false;
    }
  } 
  ORLog(kDebug) << "  Length of event header + channel lengths matches event lengths" << std::endl;
  ORLog(kDebug) << "Data record is valid " << std::endl;
  return true;  
}

void ORDGF4cEventDecoder::DumpBufferHeader()
{
  
  ORLog(kDebug) << "Dumping Buffer Header: " << std::endl
    << "BufNData: " << GetBufNData() << std::endl
    << "BufModNum: " << GetBufModNum() << std::endl
    << "RunTask: " << GetRunTask() << std::endl
    << "BufTime: " << GetBufTime() << std::endl
    << "Total Events: " << GetDGFNEvents() << std::endl;
  
  if(fDataRecord)
  {
    ORLog(kDebug) << "Dumping Data Record: " << std::endl;
    ORLog(kDebug) << "**************************************************" << std::endl;
    /*for(size_t i=2;i<LengthOf(fDataRecord); i+=2)
    {
      ORLog(kDebug) << fDataRecord[i] << std::endl;
      ORLog(kDebug) << *((UShort_t*) (fDataRecord + i)) << " "
        << *((UShort_t*) (fDataRecord + i)+ 1) << std::endl; 
    }*/
    ORLog(kDebug) << "**************************************************" << std::endl;
  }
}


//debugging: *************************************************************************


void ORDGF4cEventDecoder::Dump(UInt_t* dataRecord) //debugging 
{
  size_t iChannel, iEvent;
  size_t len; 
  UShort_t* waveform; 
  double* waveformDouble;
  
  ORLog(kDebug) << std::endl << std::endl << "ORDGF4cEventDecoder::Dump():" << std::endl ;
  iChannel = 0;
  iEvent = 0;
  if(!SetDataRecord(dataRecord)) return; 
	ORLog(kDebug) 
	  << "  Header functions: " << std::endl
	  << "    Crate = " << CrateOf() << "; card = " << CardOf() << std::endl
	  << "    The buffer is " << GetBufNData() << " words long" << std::endl
	  << "    The module number is " << GetBufModNum() << std::endl
          << "    RunTask: " << GetRunTask() << " ChanHeadLen: " << GetChanHeadLen() 
	    << std::endl
	  << "    Run Start time: " << GetBufTime() << std::endl;
	  
	  ORLog(kDebug) << "  Event functions: " << std::endl	  
          << "    Event " << iEvent << " has " << GetNChannels(iEvent) << " channels and " 
            << GetDGFEventLen(iEvent) << " words" << std::endl
	  << "    The ith active channel, where i = " << iChannel << " is channel number " 
	    << GetChannelNumber(iEvent, iChannel) << std::endl
	  << "    Event time: " << GetDGFEventTime(iEvent) << std::endl 
	  << "    There are " << GetDGFNEvents() << " events" << std::endl  
	  
	  << "  Channel functions: " << std::endl  
          << "    Channel " << iChannel << " in event "   
	    << iEvent << " has " << GetChanNData(iEvent, iChannel) << " words" << std::endl  
	  << "    Trigger time: " << GetChanTrigTime(iEvent, iChannel) << std::endl  
	  << "    Energy: " << GetChanEnergy(iEvent,iChannel) << std::endl  
	  << "    XIA PSA value: " << GetChanXiaPsa(iEvent,iChannel) << std::endl
	  << "    User PSA value: " << GetChanUserPsa(iEvent,iChannel) << std::endl
	  << "    GSLT time stamp: " << GetChanGSLT(iEvent,iChannel) << std::endl
	  << "    Real time: " << GetChanRealTime(iEvent,iChannel) << std::endl
	  << "    The waveform data has " << GetWaveformLen(iEvent,iChannel) 
	    << " words" << std::endl;
	  unsigned short wavedata[GetWaveformLen(iEvent,iChannel)];
	  waveform = wavedata;
	  double wavedataDouble[GetWaveformLen(iEvent,iChannel)];
	  waveformDouble = wavedataDouble;
	  len = GetWaveformLen(iEvent,iChannel);
	  CopyWaveformData(waveform, len, iEvent, iChannel);
	  CopyWaveformDataDouble(waveformDouble, len, iEvent, iChannel);

}

UInt_t ORDGF4cEventDecoder::GetEventWaveformPoint( size_t event, 
                                                   size_t waveformPoint ) 
{
  return (UInt_t) GetWaveformDataPointer(fEventVector[event].first,
    fEventVector[event].second)[waveformPoint];

}
