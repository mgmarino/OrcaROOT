// ORDGF4cEventDecoder.hh

#ifndef _ORDGF4cEventDecoder_hh_
#define _ORDGF4cEventDecoder_hh_

#include "ORVDigitizerDecoder.hh"
#include <vector>
#include <map>
using namespace std;

class ORDGF4cEventDecoder: public ORVDigitizerDecoder
{
  public:
    ORDGF4cEventDecoder();
    virtual ~ORDGF4cEventDecoder() {}
    enum EDG4FcEventConsts { kBufHeadLen = 6, kEventHeadLen = 3 };
    enum EListMode { kStd = 0x100, kC1 = 0x101, kC2 = 0x102, kC3 = 0x103, 
      kStdFast = 0x200, kFastC1 = 0x201, kFastC2 = 0x202, kFastC3 = 0x203 };
    
    virtual std::string GetDataObjectPath() { return "ORDGF4cModel:Event"; }
    virtual std::string GetDictionaryObjectPath() { return "ORDGF4cModel"; }
    virtual void Swap(UInt_t* dataRecord);
    /* Overloading swap, this is a 16-bit style record. */
    virtual bool SetDataRecord(UInt_t* record);
    virtual inline UInt_t CrateOf();
    virtual inline UInt_t CardOf();
    virtual inline ULong64_t BitConcat(UShort_t lo, UShort_t mid, UShort_t hi = 0x0);
       
    //Functions that return data from buffer header:
    virtual inline size_t GetBufNData();
    virtual inline UShort_t GetBufModNum();
    virtual inline UShort_t GetRunTask();
    virtual inline ULong64_t GetBufTime();
    virtual inline UShort_t GetBufTimeLo();
    virtual inline UShort_t GetBufTimeMid();
    virtual inline UShort_t GetBufTimeHi();

    //functions concerning the data format:
    virtual inline bool IsStdListMode();
    virtual inline bool IsListModeC1();
    virtual inline bool IsListModeC2();
    virtual inline bool IsListModeC3();
    virtual inline bool IsStdFastListMode();
    virtual inline bool IsFastListModeC1();
    virtual inline bool IsFastListModeC2();
    virtual inline bool IsFastListModeC3();
    virtual inline bool IsListModeAnyStdC1();
    virtual inline bool IsListModeAnyC2();
    virtual inline bool IsListModeAnyC3();
    virtual inline bool HasWaveformData() { return IsListModeAnyStdC1(); }

    //event functions:
      /* These are DGF events!  That is, an event can have many channels. */
      /* These are not the same as ORVDigitizerDecoder events. */
    virtual inline size_t GetDGFEventLen(size_t iEvent);      
    virtual inline size_t GetDGFNEvents();
    virtual inline UInt_t GetDGFEventTime(size_t iEvent);
    virtual inline UShort_t GetDGFEventTimeLo(size_t iEvent);
    virtual inline UShort_t GetDGFEventTimeHi(size_t iEvent);

    //channel functions:
    virtual size_t GetChanHeadLen();   
    virtual inline size_t GetNChannels(size_t iEvent);    
    virtual inline size_t GetChannelNumber(size_t iEvent, size_t iChannel); 
    virtual inline size_t GetChanNData(size_t iEvent, size_t iChannel);
    virtual UShort_t GetChanTrigTime(size_t iEvent, size_t iChannel);
    virtual UShort_t GetChanEnergy(size_t iEvent, size_t iChannel);
    virtual double GetChanXiaPsa(size_t iEvent, size_t iChannel);
    virtual UShort_t GetChanUserPsa(size_t iEvent, size_t iChannel);
    virtual ULong64_t GetChanGSLT(size_t iEvent, size_t iChannel);
    virtual UShort_t GetChanRealTime(size_t iEvent, size_t iChannel);
    virtual size_t GetWaveformLen(size_t iEvent, size_t iChannel);
    virtual size_t CopyWaveformData(UShort_t* waveform, size_t len, size_t iEvent, size_t iChannel);
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len, size_t iEvent, size_t iChannel);
    virtual inline const UShort_t* GetWaveformDataPointer(size_t iEvent, size_t iChannel);

    /* Channel/Card settings. */
    virtual UInt_t GetBinFactor(size_t channel);
    virtual UInt_t GetCutoffEMin(size_t channel);
    virtual double GetEnergyFlatTop(size_t channel);
    virtual double GetEnergyRiseTime(size_t channel);
    virtual Bool_t IsInSync();
    virtual double GetPSAEnd(size_t channel);
    virtual double GetPSAStart(size_t channel);
    virtual UInt_t GetRunBehavior();
    virtual Bool_t IsSyncWait();
    virtual double GetTau(size_t channel);
    virtual double GetTauSigma(size_t channel);
    virtual double GetTraceDelay(size_t channel);
    virtual double GetTraceLength(size_t channel);
    virtual double GetTriggerFlatTop(size_t channel);
    virtual double GetTriggerRiseTime(size_t channel);
    virtual double GetTriggerThreshold(size_t channel);
    virtual double GetVGain(size_t channel);
    virtual double GetVOffset(size_t channel);
    virtual UInt_t GetXWait(size_t channel);

    /* Functions satisfying the ORVDigitizerDecoder interface. */
    virtual inline double GetSamplingFrequency() {return .04;}
    virtual inline UShort_t GetBitResolution() {return 14;}
    virtual inline size_t GetNumberOfEvents() {return fEventVector.size();}
    virtual inline ULong64_t GetEventTime(size_t event); 
    virtual inline UInt_t GetEventEnergy(size_t event); 
    virtual inline UShort_t GetEventChannel(size_t event); 
    virtual inline size_t GetEventWaveformLength(size_t event); 
    virtual inline void* GetEventWaveformPointer(size_t event); 
    


    //Error checking:
    virtual bool IsValid();
    virtual bool PtrIsInDataRecord(UShort_t* ptr, bool verbose = true); 
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
    
  protected:
    virtual size_t FillChannelPtrs(size_t iEvent);
    virtual inline const UShort_t* GetChannelPointer(size_t iEvent, size_t iChannel); 

    vector<UShort_t*> fEventPtrs;				
    map< UShort_t*, vector<UShort_t*> > fChannelPtrs;
    map< UShort_t*, vector<size_t> > fChannelNumbers;
    vector< pair<size_t, size_t> > fEventVector;
};


//inline functions: ************************************************************************


inline UInt_t ORDGF4cEventDecoder::CrateOf() //returns crate # of XIA card
{ 
  return (fDataRecord[1] & 0x01e00000) >> 21; 
}

inline UInt_t ORDGF4cEventDecoder::CardOf()
{ 
  return (fDataRecord[1] & 0x001f0000) >> 16; 
}

inline ULong64_t ORDGF4cEventDecoder::BitConcat(UShort_t lo, UShort_t mid, UShort_t hi)
//Concatenates 16 bit high, middle, and low words to form a ULong64_t
{ 
  return ((ULong64_t)(lo) +  (((ULong64_t)(mid)) << 16) + (((ULong64_t)(hi)) << 32));  
}


//functions that return data from buffer header: *********************************************


inline size_t ORDGF4cEventDecoder::GetBufNData() //returns number of 16bit words in buffer
{ 
  return (*((UShort_t*) (fDataRecord+2))); 
}	

inline UShort_t ORDGF4cEventDecoder::GetBufModNum() //returns module number
{  
  return (*((UShort_t*) (fDataRecord+2) + 1)); 
} 

inline UShort_t ORDGF4cEventDecoder::GetRunTask() //returns data format
{ 
  return (*((UShort_t*) (fDataRecord+2) + 2)  - 0x1000); 
} 

inline ULong64_t ORDGF4cEventDecoder::GetBufTime() 
//returns run start time
{
  return BitConcat(GetBufTimeLo(), GetBufTimeMid(), GetBufTimeHi()); 
}

inline UShort_t ORDGF4cEventDecoder::GetBufTimeLo() 
//returns low word of run start time
{ 
  return (*((UShort_t*) (fDataRecord+2) + 5)); 
}

inline UShort_t ORDGF4cEventDecoder::GetBufTimeMid() 
//returns middle word of run start time
{ 
  return (*((UShort_t*) (fDataRecord+2) + 4)); 
}

inline UShort_t ORDGF4cEventDecoder::GetBufTimeHi() 
//returns high word of run start time
{ 
  return (*((UShort_t*) (fDataRecord+2) + 3)); 
}


//functions concerning list mode: *********************************************************


inline bool ORDGF4cEventDecoder::IsStdListMode() { return (GetRunTask() == kStd); }

inline bool ORDGF4cEventDecoder::IsListModeC1() { return (GetRunTask() == kC1); }

inline bool ORDGF4cEventDecoder::IsListModeC2() { return (GetRunTask() == kC2); }

inline bool ORDGF4cEventDecoder::IsListModeC3() { return (GetRunTask() == kC3); }

inline bool ORDGF4cEventDecoder::IsStdFastListMode() { return (GetRunTask() == kStdFast); }

inline bool ORDGF4cEventDecoder::IsFastListModeC1() { return (GetRunTask() == kFastC1); }

inline bool ORDGF4cEventDecoder::IsFastListModeC2() { return (GetRunTask() == kFastC2); }

inline bool ORDGF4cEventDecoder::IsFastListModeC3() { return (GetRunTask() == kFastC3); }

inline bool ORDGF4cEventDecoder::IsListModeAnyStdC1()
{
  return ( IsStdListMode() || IsStdFastListMode() || IsListModeC1() 
    || IsFastListModeC1() ); 
}

inline bool ORDGF4cEventDecoder::IsListModeAnyC2()
{
  return ( IsListModeC2() || IsFastListModeC2() ); 
}

inline bool ORDGF4cEventDecoder::IsListModeAnyC3()
{
  return ( IsListModeC3() || IsFastListModeC3() );
}


//event functions: ***********************************************************************

inline size_t ORDGF4cEventDecoder::GetDGFEventLen(size_t iEvent)  
//returns length of iEvent event, including header
{
  if (iEvent < GetDGFNEvents() - 1) return fEventPtrs[iEvent + 1] - fEventPtrs[iEvent];
  return fEventPtrs[0] + GetBufNData() - kBufHeadLen - fEventPtrs[iEvent];
}

inline UInt_t ORDGF4cEventDecoder::GetDGFEventTime(size_t iEvent)
//returns event start time
{
  return (UInt_t) (BitConcat(GetDGFEventTimeLo(iEvent),
    GetDGFEventTimeHi(iEvent)));  
} 

inline size_t ORDGF4cEventDecoder::GetDGFNEvents() { return fEventPtrs.size(); }

inline UShort_t ORDGF4cEventDecoder::GetDGFEventTimeLo(size_t iEvent)
//returns low word of event time
{ 
  return fEventPtrs[iEvent][2]; 
}

inline UShort_t ORDGF4cEventDecoder::GetDGFEventTimeHi(size_t iEvent)
//returns high word of event time
{ 
  return fEventPtrs[iEvent][1]; 
}


//channel functions: **********************************************************************


inline const UShort_t* ORDGF4cEventDecoder::GetChannelPointer(size_t iEvent, size_t iChannel) 
{
  return fChannelPtrs[fEventPtrs[iEvent]][iChannel];
}

inline size_t ORDGF4cEventDecoder::GetNChannels(size_t iEvent)		
//returns number of channels for the current event, unless another event is specified
{  
  return fChannelPtrs[fEventPtrs[iEvent]].size();
}

inline size_t ORDGF4cEventDecoder::GetChanNData(size_t iEvent, size_t iChannel)
{
  if (IsListModeAnyC2() || IsListModeAnyC3()) return GetChanHeadLen();
  return GetChannelPointer(iEvent,iChannel)[0];
}

inline size_t ORDGF4cEventDecoder::GetChannelNumber(size_t iEvent, size_t iChannel) 
//returns the channel number of the ith channel that was read out, where i = iChannel
{
  return fChannelNumbers[fEventPtrs[iEvent]][iChannel];
}

inline const UShort_t* ORDGF4cEventDecoder::GetWaveformDataPointer(size_t iEvent, size_t iChannel)
{
  return GetChannelPointer(iEvent, iChannel) + GetChanHeadLen();;
}

inline ULong64_t ORDGF4cEventDecoder::GetEventTime(size_t event) 
{
  return (ULong64_t) GetDGFEventTime(fEventVector[event].first);
}

inline UInt_t ORDGF4cEventDecoder::GetEventEnergy(size_t event) 
{
  return GetChanEnergy(fEventVector[event].first, fEventVector[event].second);
}

inline UShort_t ORDGF4cEventDecoder::GetEventChannel(size_t event) 
{
  return GetChannelNumber(fEventVector[event].first, fEventVector[event].second);
}

inline size_t ORDGF4cEventDecoder::GetEventWaveformLength(size_t event) 
{
  return GetWaveformLen(fEventVector[event].first, fEventVector[event].second);
}

inline void* ORDGF4cEventDecoder::GetEventWaveformPointer(size_t event) 
{
  return (void*) GetWaveformDataPointer(fEventVector[event].first,
    fEventVector[event].second);

} 

#endif

