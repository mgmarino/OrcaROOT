// ORKatrinFLTWaveformDecoder.hh

#ifndef _ORKatrinFLTWaveformDecoder_hh_
#define _ORKatrinFLTWaveformDecoder_hh_

#include "ORVDigitizerDecoder.hh"
#include <vector>
#include <map>
using namespace std;

/** Decodes the binary Orca data format and writes it into a ROOT TFile.
  * The binary data format description is in \file ORKatrinFLTDecoder.m .
  * In  \file ORKatrinFLTModel.m in in - (NSDictionary*) dataRecordDescription
  * the entries in the data dictionary define the data key and its according
  * selector of the decoder. In this case it is "KatrinFLTWaveForm". The decoder of
  * this dictionary is defined as ORKatrinFLTDecoderForEnergy.
  * The source code (in \file ORKatrinFLTDecoder.m) of this method (ORKatrinFLTDecoderForEnergy)
  * holds the description of this format.
  *
  * This format is recognized by the return value of GetDataObjectPath() which is
  * "ORKatrinFLTModel:KatrinFLTWaveForm".
  */ //-tb- 2008-02-6
class ORKatrinFLTWaveformDecoder: public ORVDigitizerDecoder
{
  public:
    ORKatrinFLTWaveformDecoder();
    virtual ~ORKatrinFLTWaveformDecoder() {}
    enum EKatrinFLTWaveformConsts {kBufHeadLen = 9, 
                                  kWaveformLength = 1024,
                                  kNumFLTChannels = 22};
    
    virtual std::string GetDataObjectPath() { return "ORKatrinFLTModel:KatrinFLTWaveForm"; }  
    //!< KatrinFLTWaveForm is the key in ORKatrinFLTModel.m in - (NSDictionary*) dataRecordDescription -tb- 2008-02-12

    virtual bool SetDataRecord(UInt_t* record);
       
    //Functions that return data from buffer header:
    virtual inline UInt_t GetSec();
    virtual inline UInt_t GetSubSec();
    virtual inline UInt_t GetChannelMap();
    virtual inline UShort_t GetChannel();
    virtual inline UShort_t GetEventID();
    virtual inline UShort_t GetPageNumber();
    virtual inline UInt_t GetEnergy();
    virtual inline UInt_t GetResetSec();
    virtual inline UInt_t GetResetSubSec();

    // Waveform Functions
    virtual inline size_t GetWaveformLen() {return kWaveformLength;} 
    virtual inline UShort_t* GetWaveformDataPointer();
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);

    /* Satisfying ORVDigitizerDecoder interface. */
    virtual double GetSamplingFrequency() {return .01;}
    virtual UShort_t GetBitResolution() {return 14;} 
    virtual size_t GetNumberOfEvents() {return 1;}
    virtual ULong64_t GetEventTime(size_t /*event*/); 
    virtual UInt_t GetEventEnergy(size_t /*event*/) {return GetEnergy();}
    virtual UShort_t GetEventChannel(size_t /*event*/) {return GetChannel();}
    virtual size_t GetEventWaveformLength(size_t /*event*/) 
      { return GetWaveformLen(); }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, 
                                          size_t waveformPoint )
      { return (UInt_t) GetWaveformDataPointer()[waveformPoint]; }
 
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
  return (fDataRecord[4] & 0x3FFFFF);
}

inline UShort_t ORKatrinFLTWaveformDecoder::GetEventID()
{
  return ( fDataRecord[5] & 0x3FF );
}

inline UShort_t ORKatrinFLTWaveformDecoder::GetPageNumber()
{
  return ( fDataRecord[5] & 0x1FF0000 ) >> 16;
}

inline UInt_t ORKatrinFLTWaveformDecoder::GetEnergy()
{
  return (fDataRecord[6]);
}

inline UInt_t ORKatrinFLTWaveformDecoder::GetResetSec()
{
  return (fDataRecord[7]);
}

inline UInt_t ORKatrinFLTWaveformDecoder::GetResetSubSec()
{
  return (fDataRecord[8]);
}

inline UShort_t ORKatrinFLTWaveformDecoder::GetChannel()
{
  return ( fDataRecord[4] & 0xFF000000 ) >> 24;
}

inline UShort_t* ORKatrinFLTWaveformDecoder::GetWaveformDataPointer()
{
  return (UShort_t*)(fDataRecord + kBufHeadLen);
}

inline ULong64_t ORKatrinFLTWaveformDecoder::GetEventTime( size_t )
{
  return ((ULong64_t)GetSec() << 32 ) + (ULong64_t)GetSubSec();
}

#endif

