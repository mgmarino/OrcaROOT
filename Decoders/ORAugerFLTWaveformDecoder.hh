// ORAugerFLTWaveformDecoder.hh

#ifndef _ORAugerFLTWaveformDecoder_hh_
#define _ORAugerFLTWaveformDecoder_hh_

#include "ORVDataDecoder.hh"
#include <vector>
#include <map>
using namespace std;

class ORAugerFLTWaveformDecoder: public ORVDataDecoder
{
  public:
    ORAugerFLTWaveformDecoder();
    virtual ~ORAugerFLTWaveformDecoder() {}
    enum EAugerFLTWaveformConsts {kBufHeadLen = 7, 
                                  kWaveformLength = 1024,
                                  kNumFLTChannels = 22};
    
    virtual std::string GetDataObjectPath() { return "ORAugerFLTModel:AugerFLTWaveForm"; }  
    virtual bool SetDataRecord(UInt_t* record);
    virtual inline UShort_t CrateOf();
    virtual inline UShort_t CardOf();
       
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
 
    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
    
  protected:
    UInt_t* fDataRecord;
};

//inline functions: ************************************************************************

inline UShort_t ORAugerFLTWaveformDecoder::CrateOf() //returns crate # of card
{ 
  return (UShort_t)((fDataRecord[1] & 0x01e00000) >> 21); 
}

inline UShort_t ORAugerFLTWaveformDecoder::CardOf()
{ 
  return (UShort_t)((fDataRecord[1] & 0x001f0000) >> 16); 
}


inline UInt_t ORAugerFLTWaveformDecoder::GetSec()
{
  return (fDataRecord[2]);
}
 
inline UInt_t ORAugerFLTWaveformDecoder::GetSubSec()
{
  return (fDataRecord[3]);
}

inline UInt_t ORAugerFLTWaveformDecoder::GetChannelMap()
{
  return (fDataRecord[4]);
}

inline UInt_t ORAugerFLTWaveformDecoder::GetEventID()
{
  return (fDataRecord[5]);
}

inline UInt_t ORAugerFLTWaveformDecoder::GetEnergy()
{
  return (fDataRecord[6]);
}

inline UShort_t ORAugerFLTWaveformDecoder::GetChannel()
{
  for(size_t i=0; i<kNumFLTChannels; i++) {
    if((0x00000001<<i) & GetChannelMap()) return i; 
  }
  return (UShort_t)(-1);
  /* If we get here, there's an error*/
}

inline const UShort_t* ORAugerFLTWaveformDecoder::GetWaveformDataPointer()
{
  return (UShort_t*)(fDataRecord + kBufHeadLen);
}
#endif

