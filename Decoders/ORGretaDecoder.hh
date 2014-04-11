// ORGretaDecoder.hh

#ifndef _ORGretaDecoder_hh_
#define _ORGretaDecoder_hh_

#include "ORVDigitizerDecoder.hh"
#include "ORUtils.hh"
using ORUtils::BitConcat;

class ORGretaDecoder: public ORVDigitizerDecoder
{
  public:
    ORGretaDecoder();
    virtual ~ORGretaDecoder() {}
    enum EGretaConsts { kBufHeadLen = 6 };
    enum EGretaPolarity { kNone = 0,
                          kPos,
                          kNeg,
                          kBoth };

    enum EGretaTriggerMode { kInternal = 0,
                             kExternal,
                             kValidation };
    
    virtual std::string GetDataObjectPath() { return "ORGretinaModel:Gretina"; }  
    virtual std::string GetDictionaryObjectPath() { return "ORGretinaModel"; }  
    //virtual void Swap(UInt_t* dataRecord);
    /* Overloading swap, this is a 16-bit style record. */
    virtual bool SetDataRecord(UInt_t* record);
       
    //Functions that return data from buffer header:
    virtual inline size_t GetBufHeadLen() {return (size_t) kBufHeadLen;}
    virtual inline UShort_t GetBoardId();
    virtual inline UShort_t GetChannelNum();
    virtual inline UShort_t GetPacketLength();
    virtual inline UShort_t GetLEDExtTimeStampLo();
    virtual inline UShort_t GetLEDExtTimeStampMed();
    virtual inline UShort_t GetLEDExtTimeStampHi();
    virtual inline ULong64_t GetLEDExtTimeStamp();
    virtual inline UShort_t GetEnergyLo();
    virtual inline UShort_t GetEnergyHi();
    virtual inline UInt_t GetEnergy();
    virtual inline Bool_t IsLEDCrossingNeg();
    virtual inline Bool_t IsExternalTrigFlag();
    virtual inline Bool_t IsCFDCrossingFlag();
    virtual inline Bool_t IsPileupFlag();
    virtual inline UShort_t GetCFDTimeStampLo();
    virtual inline UShort_t GetCFDTimeStampMed();
    virtual inline UShort_t GetCFDTimeStampHi();
    virtual inline ULong64_t GetCFDTimeStamp();
    virtual inline UInt_t GetCFDPointOne();
    virtual inline UInt_t GetCFDPointTwo();

    // Waveform Functions
    virtual inline size_t GetWaveformLen(); 
    virtual size_t CopyWaveformData(UShort_t* waveform, size_t len);
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);
    virtual size_t CopyWaveformDataShort(short* waveform, size_t len);
    virtual inline UInt_t* GetWaveformDataPointer();
 
    /* Functions that return information about card/channel settings. */
    /* These are static throughout a run, so a processor should take  * 
     * advantage of this and maybe not query during each record.      */
    virtual UInt_t GetBaseAddress();
    virtual UInt_t GetCFDDelay();
    virtual UInt_t GetCFDFraction();
    virtual UInt_t GetCFDThreshold();
    virtual UInt_t GetCollectionTime();
    virtual UInt_t GetDataDelay();
    virtual UInt_t GetDataLength();
    virtual Bool_t IsDebugModeOn();
    virtual Bool_t IsEnabled();
    virtual UInt_t GetExternalTriggerLength();
    virtual UInt_t GetExternalWindow();
    virtual UInt_t GetIntegrationTime();
    virtual UInt_t GetLEDThreshold();
    virtual UInt_t GetNoiseWindow();
    virtual Bool_t IsPileUpEnabled();
    virtual UInt_t GetPileUpWindow();
    virtual EGretaPolarity GetPolarity();
    virtual EGretaTriggerMode GetTriggerMode();

    /* Functions satisfying the ORVDigitizerDecoder interface. */
    virtual inline double GetSamplingFrequency() { return .1; }
    virtual inline UShort_t GetBitResolution() { return 12; }
    virtual inline size_t GetNumberOfEvents() { return 1; }
    virtual inline ULong64_t GetEventTime(size_t /*event*/) {return GetLEDExtTimeStamp();}
    virtual inline UInt_t GetEventEnergy(size_t /*event*/) {return GetEnergy();}
    virtual inline UShort_t GetEventChannel(size_t /*event*/) {return GetChannelNum();}
    virtual size_t GetEventWaveformLength(size_t /*event*/) 
      { return GetWaveformLen(); }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, 
                                          size_t waveformPoint );
    
    virtual UInt_t GetEventFlags(size_t /*event*/);
    

    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
    
  protected:
    /* GetRecordOffset() returns how many words the record is offset from the 
       beginning.  This is useful when additional headers are added. */
    virtual inline size_t GetRecordOffset() {return 2;}
    // This is a bit mask to grab the correct values from the waveform data.
    UInt_t fBitMask;
};

//inline functions: ************************************************************************

inline UShort_t ORGretaDecoder::GetBoardId()
{
  return (UShort_t)(fDataRecord[GetRecordOffset()] & 0xffff) >> 3;
}

inline UShort_t ORGretaDecoder::GetChannelNum()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()] & 0x7);
}

inline UShort_t ORGretaDecoder::GetPacketLength()
{
  return (UShort_t) (( fDataRecord[GetRecordOffset()] & 0x7FF0000 )  >> 16);
}

inline UShort_t ORGretaDecoder::GetLEDExtTimeStampLo()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+1] & 0xffff);
}

inline UShort_t ORGretaDecoder::GetLEDExtTimeStampMed()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+1]  >> 16);
}

inline UShort_t ORGretaDecoder::GetLEDExtTimeStampHi()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+2] & 0xffff);
}

inline ULong64_t ORGretaDecoder::GetLEDExtTimeStamp()
{
  return BitConcat(GetLEDExtTimeStampLo(), GetLEDExtTimeStampMed(), GetLEDExtTimeStampHi());
}

inline UShort_t ORGretaDecoder::GetEnergyLo()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+2] >> 16); 
}

inline UShort_t ORGretaDecoder::GetEnergyHi()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+3] & 0x7f); 
}

inline UInt_t ORGretaDecoder::GetEnergy()
{
  return (UInt_t) BitConcat(GetEnergyLo(), GetEnergyHi());
}

inline Bool_t ORGretaDecoder::IsLEDCrossingNeg()
{
  return ((fDataRecord[GetRecordOffset()+3]  >> 12) & 0x1) == 1;
}

inline Bool_t ORGretaDecoder::IsExternalTrigFlag()
{
  return ((fDataRecord[GetRecordOffset()+3]  >> 13) & 0x1) == 1;
}

inline Bool_t ORGretaDecoder::IsCFDCrossingFlag()
{
  return ((fDataRecord[GetRecordOffset()+3] >> 14) & 0x1) == 1;
}

inline Bool_t ORGretaDecoder::IsPileupFlag()
{
  return ((fDataRecord[GetRecordOffset()+3]  >> 15) & 0x1) == 1;
}

inline UShort_t ORGretaDecoder::GetCFDTimeStampLo()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+3]  >> 16); 
}

inline UShort_t ORGretaDecoder::GetCFDTimeStampMed()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+4] & 0xffff); 
}

inline UShort_t ORGretaDecoder::GetCFDTimeStampHi()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+4]  >> 16); 
}

inline ULong64_t ORGretaDecoder::GetCFDTimeStamp()
{
  return BitConcat(GetCFDTimeStampLo(), GetCFDTimeStampMed(), GetCFDTimeStampHi());
}

inline UInt_t ORGretaDecoder::GetCFDPointOne()
{
  return (fDataRecord[GetRecordOffset()+5] & 0xffff); 
}

inline UInt_t ORGretaDecoder::GetCFDPointTwo()
{
  return (fDataRecord[GetRecordOffset()+5] >> 16); 
}

inline size_t ORGretaDecoder::GetWaveformLen()
{
  // returns Waveform length in number of shorts
  return (GetPacketLength() - GetBufHeadLen())*2;
}

inline UInt_t* ORGretaDecoder::GetWaveformDataPointer()
{
  return (fDataRecord + GetRecordOffset() + GetBufHeadLen());
}

#endif

