// ORKatrinFLTWaveformDecoder.hh

#ifndef _ORKatrinFLTWaveformDecoder_hh_
#define _ORKatrinFLTWaveformDecoder_hh_

#include "ORVDigitizerDecoder.hh"
#include <vector>
#include <map>
using namespace std;

class ORKatrinFLTWaveformDecoder: public ORVDigitizerDecoder
{
  public:
    ORKatrinFLTWaveformDecoder();
    virtual ~ORKatrinFLTWaveformDecoder() {}
    enum EKatrinFLTWaveformConsts {kBufHeadLen = 7, 
                                  kWaveformLength = 1024,
                                  kNumFLTChannels = 22};
    
    virtual std::string GetDataObjectPath() { return "ORKatrinFLTModel:KatrinFLTWaveForm"; }  
    virtual bool SetDataRecord(UInt_t* record);
       
    //Functions that return data from buffer header:
    virtual inline UInt_t GetSec();
    virtual inline UInt_t GetSubSec();
    virtual inline UInt_t GetChannelMap();
    virtual inline UShort_t GetChannel();
    virtual inline UInt_t GetEventID();
    virtual inline UInt_t GetEnergy();

    // Waveform Functions
    virtual inline size_t GetWaveformLen() {return kWaveformLength;} 
    virtual inline const UShort_t* GetWaveformDataPointer();
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);

    /* Satisfying ORVDigitizerDecoder interface. */
    virtual double GetSamplingFrequency() {return .01;}
    virtual UShort_t GetBitResolution() {return 14;} 
    virtual size_t GetNumberOfEvents() {return 1;}
    virtual ULong64_t GetEventTime(size_t /*event*/) {return 0;}
      /*This card doesn't store event time information. */
    virtual UInt_t GetEventEnergy(size_t /*event*/) {return GetEnergy();}
    virtual UShort_t GetEventChannel(size_t /*event*/) {return GetChannel();}
    virtual size_t GetEventWaveformLength(size_t /*event*/) 
      {return GetWaveformLen();}
    virtual void* GetEventWaveformPointer(size_t /*event*/) 
      {return (void*)GetWaveformDataPointer();} 
 
    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
    
};

//inline functions: ************************************************************************

inline UInt_t ORKatrinFLTWaveformDecoder::GetSec()
{
  return (fDataRecord[2]);
}
 
inline UInt_t ORKatrinFLTWaveformDecoder::GetSubSec()
{
  return (fDataRecord[3]);
}

inline UInt_t ORKatrinFLTWaveformDecoder::GetChannelMap()
{
  return (fDataRecord[4]);
}

inline UInt_t ORKatrinFLTWaveformDecoder::GetEventID()
{
  return (fDataRecord[5]);
}

inline UInt_t ORKatrinFLTWaveformDecoder::GetEnergy()
{
  return (fDataRecord[6]);
}

inline UShort_t ORKatrinFLTWaveformDecoder::GetChannel()
{
  for(size_t i=0; i<kNumFLTChannels; i++) {
    if((0x00000001<<i) & GetChannelMap()) return i; 
  }
  return (UShort_t)(-1);
  /* If we get here, there's an error*/
}

inline const UShort_t* ORKatrinFLTWaveformDecoder::GetWaveformDataPointer()
{
  return (UShort_t*)(fDataRecord + kBufHeadLen);
}
#endif

