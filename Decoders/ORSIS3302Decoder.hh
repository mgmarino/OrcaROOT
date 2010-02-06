// ORSIS3302Decoder.hh

#ifndef _ORSIS3302Decoder_hh_
#define _ORSIS3302Decoder_hh_

#include "ORVDigitizerDecoder.hh"

class ORSIS3302Decoder: public ORVDigitizerDecoder
{
  public:
    ORSIS3302Decoder();
    virtual ~ORSIS3302Decoder() {}
    enum ESIS3302Consts { kOrcaHeaderLen = 4,
                          kBufferHeaderLen = 2,
                          kBufferTrailerLen = 4 };

    enum ESIS3302TriggerMode { kInternal = 0,
                             kExternal,
                             kValidation };
    
    virtual std::string GetDataObjectPath() { return "ORGretinaModel:Gretina"; }  
    virtual std::string GetDictionaryObjectPath() { return "ORGretinaModel"; }  
    virtual bool SetDataRecord(UInt_t* record);
       
    //Functions that return data from buffer header:
    virtual inline size_t GetBufHeadLen() 
      { return (size_t) (kBufferHeaderLen + kOrcaHeaderLen);}
    virtual inline size_t GetTrailerLen() 
      { return (size_t) (kBufferTrailerLen);}
    virtual inline UShort_t GetBoardId();
    virtual inline UShort_t GetChannelNum();
    virtual inline UShort_t GetTimeStampLo();
    virtual inline UShort_t GetTimeStampMed();
    virtual inline UShort_t GetTimeStampHi();
    virtual inline ULong64_t GetTimeStamp();

    // Returns maximum energy found in the gate
    virtual inline UInt_t GetEnergyMax();
    // Returns energy at the first point of the energy gate 
    virtual inline UInt_t GetEnergyInitial();
    virtual inline UInt_t GetFlags();
    virtual inline UInt_t GetTrailer();


    virtual inline Bool_t IsPileupFlag()
      { return ((GetFlags() & 0x80000000) == 0x80000000); }
    virtual inline Bool_t IsRetriggerFlag()
      { return ((GetFlags() & 0x40000000) == 0x40000000); }
    virtual inline Bool_t IsADCNPlusOneTriggerFlag()
      { return ((GetFlags() & 0x20000000) == 0x20000000); }
    virtual inline Bool_t IsADCNMinusOneTriggerFlag()
      { return ((GetFlags() & 0x10000000) == 0x10000000); }
    virtual inline Bool_t IsTriggerFlag()
      { return ((GetFlags() & 0x1) == 0x1); }

    virtual inline UShort_t GetFastTriggerCounter()
      { return (UShort_t)((GetFlags() & 0x0F000000) >> 24); }
    // Waveform Functions

    // Waveform length in number of 16-bit words
    virtual inline size_t GetWaveformLen() { return fDataRecord[2]*2; }
    virtual size_t CopyWaveformData(UShort_t* waveform, size_t len);
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);
    virtual inline UInt_t* GetWaveformDataPointer();
 
    // Energy Waveform length in number of 32-bit words
    virtual inline size_t GetEnergyWaveformLen() {return fDataRecord[2]; } 
    virtual size_t CopyEnergyWaveformDataDouble(double* waveform, size_t len);
    virtual inline UInt_t* GetEnergyWaveformDataPointer();

    /* Functions that return information about card/channel settings. */
    /* These are static throughout a run, so a processor should take  * 
     * advantage of this and maybe not query during each record.      */

    /* Functions satisfying the ORVDigitizerDecoder interface. */
    virtual inline double GetSamplingFrequency() { return .1; }
    virtual inline UShort_t GetBitResolution() { return 16; }
    virtual inline size_t GetNumberOfEvents() { return 1; }
    virtual inline ULong64_t GetEventTime(size_t /*event*/) 
      { return GetTimeStamp(); }
    virtual inline UInt_t GetEventEnergy(size_t /*event*/) 
      { return GetEnergyMax(); }
    virtual inline UShort_t GetEventChannel(size_t /*event*/) 
      { return GetChannelNum(); }
    virtual size_t GetEventWaveformLength(size_t /*event*/) 
      { return GetWaveformLen(); }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, 
                                          size_t waveformPoint );
    
    virtual UInt_t GetEventFlags(size_t /*event*/) { return GetFlags(); }
    

    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
    
  protected:
    /* GetRecordOffset() returns how many words the record is offset from the 
       beginning.  This is useful when additional headers are added. */
    virtual inline size_t GetRecordOffset() {return kOrcaHeaderLen;}
    virtual inline size_t GetTrailerOffset() 
      { return GetBufHeadLen() + GetWaveformLen()/2 + GetEnergyWaveformLen(); }
};

//inline functions: ************************************************************************

inline UShort_t ORSIS3302Decoder::GetBoardId()
{
  // The bit shift gets rid of the channel information
  return (UShort_t)(fDataRecord[GetRecordOffset()] & 0xffff) >> 2;
}

inline UShort_t ORSIS3302Decoder::GetChannelNum()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()] & 0x3);
}


inline UShort_t ORSIS3302Decoder::GetTimeStampLo()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+1] & 0xffff);
}

inline UShort_t ORSIS3302Decoder::GetTimeStampMed()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()+1]  >> 16);
}

inline UShort_t ORSIS3302Decoder::GetTimeStampHi()
{
  return (UShort_t) (fDataRecord[GetRecordOffset()] >> 16);
}

inline ULong64_t ORSIS3302Decoder::GetTimeStamp()
{

  return BitConcat(GetTimeStampLo(), 
                   GetTimeStampMed(), 
                   GetTimeStampHi());  
}

inline UInt_t* ORSIS3302Decoder::GetWaveformDataPointer()
{
  return (fDataRecord + GetRecordOffset() + GetBufHeadLen());
}

inline UInt_t* ORSIS3302Decoder::GetEnergyWaveformDataPointer()
{
  return (fDataRecord + GetRecordOffset() + GetBufHeadLen() 
          + GetWaveformLen()/2);
}

inline UInt_t ORSIS3302Decoder::GetEnergyMax()
      { return fDataRecord[GetTrailerOffset()]; }

    // Returns energy at the first point of the energy gate 
inline UInt_t ORSIS3302Decoder::GetEnergyInitial()
      { return fDataRecord[GetTrailerOffset() + 1]; }

inline UInt_t ORSIS3302Decoder::GetFlags()
      { return fDataRecord[GetTrailerOffset() + 2]; }

inline UInt_t ORSIS3302Decoder::GetTrailer()
      { return fDataRecord[GetTrailerOffset() + 3]; }


#endif

