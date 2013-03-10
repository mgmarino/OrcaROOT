// ORSIS3302GenericDecoder.cc

#include "ORSIS3302GenericDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"


//**************************************************************************************

ORSIS3302GenericDecoder::ORSIS3302GenericDecoder()
 : fNumberOfEvents(0), fWaveformDataPtr(NULL)
{ 
}

bool ORSIS3302GenericDecoder::SetDataRecord(UInt_t* dataRecord) 
{
    fDataRecord = dataRecord;
    UShort_t card = CardOf(); 
    UShort_t crate = CrateOf(); 
    UShort_t channel = GetEventChannel(0); 
    UInt_t lenOfRecord = LengthOf(dataRecord);
    UInt_t len = GetTotalWaveformLength(); 
    fNumberOfEvents = 1;
    ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
    if(!IsValid()) {
        ORLog(kDebug) << "SetDataRecord(): data record is not valid" << std::endl;
        fDataRecord = NULL;
        fNumberOfEvents = 0;
        return false;
    }
    ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
    if (len == kOrcaHeaderLen) return true; 
    if (lenOfRecord - kOrcaHeaderLen == len) {
	fWaveformDataPtr =  fDataRecord + GetRecordOffset();
        return true;
    } else {
      WFKey key = (( 0xFF & crate) << 24) + 
                  (( 0xFF & card ) << 16) +
                  (( 0xFF & channel) << 8); 
      WFVec& vec = fWFCache[key].first;
      bool&  isFullWF = fWFCache[key].second;
      if (vec.size() + (lenOfRecord - kOrcaHeaderLen) == len) {
        // Append
	vec.insert(vec.end(), &dataRecord[4], &dataRecord[4] + (lenOfRecord -
          kOrcaHeaderLen)); 
        fWaveformDataPtr = &vec[0];
        isFullWF = true;
      } else {
        if (vec.size() == 0) {
          // Means this is the first cache entry, be sure we have set the WF flag
          isFullWF = false;
        }
        if (isFullWF) {
          // Means we are resetting a previously full waveform
          isFullWF = false;
          vec.resize(0);
        }
	vec.insert(vec.begin(), &dataRecord[4], &dataRecord[4] + (lenOfRecord -
          kOrcaHeaderLen)); 
        fWaveformDataPtr = NULL;
        fNumberOfEvents = 0;
      } 
    }
    
    return true;
}



//Channel functions: ******************************************************************

//copies the waveform data to the array pointed to by
//waveform, which is of length len
size_t ORSIS3302GenericDecoder::CopyWaveformData(UShort_t* waveform, size_t len)
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
size_t ORSIS3302GenericDecoder::CopyWaveformDataDouble(double* waveform, size_t len)
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


/* Card/Channel settings, parameters. */

//Error checking: **********************************************************************



bool ORSIS3302GenericDecoder::IsValid() 
{ 
    ORLog(kDebug) << "IsValid(): starting... " << std::endl;
    if(IsShort(fDataRecord)) { 
        ORLog(kError) << "Data file is short" << std::endl; 
        return false;
    } 
    return true;
}

//debugging: *************************************************************************


void ORSIS3302GenericDecoder::Dump(UInt_t* dataRecord) //debugging 
{
    size_t iChannel, iEvent;
    
    ORLog(kDebug) << std::endl << std::endl << "ORSIS3302GenericDecoder::Dump():" << std::endl ;
    iChannel = 0;
    iEvent = 0;
    if(!SetDataRecord(dataRecord)) return; 
    ORLog(kDebug) 
    << "  Header functions: " << std::endl
    << "    Crate = " << CrateOf() << "; card = " << CardOf() << std::endl
    << std::endl
    << "    The channel is " << GetEventChannel(0) << std::endl
    << std::endl  
    << "    The waveform data has " << GetWaveformLen() 
    << " (16-bit) words" << std::endl;
    
}

UInt_t ORSIS3302GenericDecoder::GetEventWaveformPoint( size_t /*event*/, 
                                               size_t waveformPoint )
{
    if ( waveformPoint % 2 == 0 ) {
        return GetWaveformDataPointer()[waveformPoint/2] & 0xFFFF; 
    } else {
        return ( GetWaveformDataPointer()[waveformPoint/2] >> 16); 
    } 
}


UInt_t ORSIS3302GenericDecoder::GetAveragingForChannel( size_t chan )
{
  return GetIntValueFromKeyArray("averagingSettings", 
    CrateOf(), CardOf(), chan/2);
}
ORSIS3302GenericDecoder::EClockType 
  ORSIS3302GenericDecoder::GetClockType()
{
  return (EClockType) GetIntValueFromKey("clockSource", 
    CrateOf(), CardOf());
}

double ORSIS3302GenericDecoder::GetSamplingFrequency()
{
  switch(GetClockType()) {
    case k100MHzBad:
    case k100MHz: return 0.1;  
    case k50MHz: return 0.05;
    case k25MHz: return 0.025;
    case k10MHz: return 0.01;
    case k1MHz: return  0.001;
    case kExternalRand:
    case kExternal:
    default: return 1.;
  } 
}
