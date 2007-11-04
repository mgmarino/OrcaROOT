// ORAcqirisDC440Decoder.hh

#ifndef _ORAcqirisDC440Decoder_hh_
#define _ORAcqirisDC440Decoder_hh_

#include "ORVDataDecoder.hh"
#include <vector>
#include <map>
using namespace std;

class ORAcqirisDC440Decoder: public ORVDataDecoder
{
  public:
    ORAcqirisDC440Decoder();
    virtual ~ORAcqirisDC440Decoder() {}
    enum EAcqirisDC440Consts {kBufHeadLen = 6};
    
    virtual std::string GetDataObjectPath() { return "ORAcqirisCD440Model:Waveform"; }  
    virtual void Swap(UInt_t* dataRecord);
    /* Overloading swap, this is a 16-bit style record. */
    virtual bool SetDataRecord(UInt_t* record);
    virtual inline UInt_t CrateOf();
    virtual inline UInt_t CardOf();
    virtual inline ULong64_t BitConcat(UInt_t lo, UInt_t hi);
       
    //Functions that return data from buffer header:
    virtual inline UShort_t GetChannelNum();
    virtual inline UInt_t GetTimeStampLo();
    virtual inline UInt_t GetTimeStampHi();
    virtual inline ULong64_t GetTimeStamp();

    // Waveform Functions
    virtual inline UInt_t GetIndexOffset();
    virtual inline size_t GetWaveformLen(); 
    virtual inline const Short_t* GetWaveformDataPointer();
      /* Returns number of shorts in the waveform. */
    virtual size_t CopyWaveformData(Short_t* waveform, size_t len);
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);
 
    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
    
  protected:
    UInt_t* fDataRecord;
};

//inline functions: ************************************************************************

inline UInt_t ORAcqirisDC440Decoder::CrateOf() //returns crate # of AcqirisDC440 card
{ 
  return (fDataRecord[1] & 0x01e00000) >> 21; 
}

inline UInt_t ORAcqirisDC440Decoder::CardOf()
{ 
  return (fDataRecord[1] & 0x001f0000) >> 16; 
}

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

inline const Short_t* ORAcqirisDC440Decoder::GetWaveformDataPointer()
{
  return (Short_t*) (fDataRecord + kBufHeadLen + GetIndexOffset());
}
#endif

