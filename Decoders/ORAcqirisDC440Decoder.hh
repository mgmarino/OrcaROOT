// ORAcqirisDC440Decoder.hh

#ifndef _ORAcqirisDC440Decoder_hh_
#define _ORAcqirisDC440Decoder_hh_

#include "ORVDigitizerDecoder.hh"

class ORAcqirisDC440Decoder: public ORVDigitizerDecoder
{
  public:
    ORAcqirisDC440Decoder();
    virtual ~ORAcqirisDC440Decoder() {}
    enum EAcqirisDC440Consts {kBufHeadLen = 6};
    
    enum EAcqirisDC440TriggerSlope { kPositive = 0,
                                     kNegative,
                                     kOutOfWindow,
                                     kIntoWindow,
                                     kHFDivide,
                                     kSpikeStretcher };
  
    enum EAcqirisDC440TriggerCoupling { kDC = 0,
                                        kAC,
                                        kHFReject,
                                        kDC_50_Ohm,
                                        kAC_50_Ohm }; 

    enum EAcqirisDC440TriggerSource { kExternal = 0,
                                      kChannelOne, 
                                      kChannelTwo }; 
    
    enum EAcqirisDC440VerticalCoupling { kVC_Ground = 0,
                                         kVC_DC_1_MOhm,
                                         kVC_AC_1_MOhm,
                                         kVC_DC_50_Ohm,
                                         kVC_AC_50_Ohm };
                                       
    virtual std::string GetDataObjectPath() { return "ORAcqirisDC440Model:Waveform"; }  
    virtual std::string GetDictionaryObjectPath() { return "ORAcqirisDC440Model"; }  
    virtual void Swap(UInt_t* dataRecord);
    /* Overloading swap, this is a 16-bit style record. */
    virtual bool SetDataRecord(UInt_t* record);
    virtual inline ULong64_t BitConcat(UInt_t lo, UInt_t hi);
       
    //Functions that return data from buffer header:
    virtual inline UShort_t GetChannelNum();
    virtual inline UInt_t GetTimeStampLo();
    virtual inline UInt_t GetTimeStampHi();
    virtual inline ULong64_t GetTimeStamp();

    // Waveform Functions
    virtual inline UInt_t GetIndexOffset();
    virtual inline size_t GetWaveformLen(); 
    virtual inline UShort_t* GetWaveformDataPointer();
      /* Returns number of shorts in the waveform. */
    virtual size_t CopyWaveformData(Short_t* waveform, size_t len);
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);
 
    /* Functions that return information about card/channel settings. */
    /* These are static throughout a run, so a processor should take  * 
     * advantage of this and maybe not query during each record.      */
    virtual double GetDelayTime();
    virtual double GetFullScale();
    virtual UInt_t GetNumberOfSamples();
    virtual double GetSampleInterval();
    virtual EAcqirisDC440TriggerCoupling GetTriggerCoupling();
    virtual double GetTriggerLevel();
    virtual EAcqirisDC440TriggerSlope GetTriggerSlope();
    virtual EAcqirisDC440TriggerSource GetTriggerSource();
    virtual EAcqirisDC440VerticalCoupling GetVerticalCoupling();
    virtual double GetVerticalOffset();

    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
    

    /* Adhering to ORVDigitizerDecoder interface. */
    virtual double GetSamplingFrequency();
    virtual UShort_t GetBitResolution() {return 12;}
    virtual size_t GetNumberOfEvents() {return 1;}
    virtual ULong64_t GetEventTime(size_t /*event*/) { return GetTimeStamp(); }
      /* This card doesn't calculate energy. */
    virtual UInt_t GetEventEnergy(size_t /*event*/) { return 0; }
    virtual UShort_t GetEventChannel(size_t /*event*/) { return GetChannelNum();}
    virtual size_t GetEventWaveformLength(size_t /*event*/) 
      { return GetWaveformLen(); }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, 
                                          size_t waveformPoint );

};

//inline functions: ************************************************************************

inline ULong64_t ORAcqirisDC440Decoder::BitConcat(UInt_t lo, UInt_t hi)
//Concatenates 32 bit high and low words to form a ULong64_t
{ 
  return ((ULong64_t)(lo) + (((ULong64_t)(hi)) << 32));  
}


inline UShort_t ORAcqirisDC440Decoder::GetChannelNum()
{
  return (UShort_t) ((fDataRecord[1] & 0xff00) >> 8);
}

inline UInt_t ORAcqirisDC440Decoder::GetTimeStampLo()
{
  return (fDataRecord[2]);
}

inline UInt_t ORAcqirisDC440Decoder::GetTimeStampHi()
{
  return (fDataRecord[3]);
}

inline ULong64_t ORAcqirisDC440Decoder::GetTimeStamp()
{
  return BitConcat(GetTimeStampLo(), GetTimeStampHi());
}

inline UInt_t ORAcqirisDC440Decoder::GetIndexOffset()
{
  return (fDataRecord[4]);
}

inline size_t ORAcqirisDC440Decoder::GetWaveformLen()
{
  /* returns Waveform length in number of shorts */
  return (fDataRecord[5]);
}

inline UShort_t* ORAcqirisDC440Decoder::GetWaveformDataPointer()
{
  return (UShort_t*)(fDataRecord + kBufHeadLen + GetIndexOffset());
}
#endif

