// ORMotionNodeDecoder.hh

#ifndef _ORMotionNodeDecoder_hh_
#define _ORMotionNodeDecoder_hh_

#include "ORVDigitizerDecoder.hh"
/*----------------------------------------------
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
 ^^^^ ^^^^ ^^^^ ^^-----------------------data id
                  ^^ ^^^^ ^^^^ ^^^^ ^^^^-length in longs
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
                  ^^---------------------traceid (0=x,1=y,2=z)
					 ^^^^ ^^^^ ^^^^-device
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx Unix Time
 the  trace follows and fills out the record length (floats encoded as longs)
 ------------------------------------------------*/

class ORMotionNodeDecoder: public ORVDigitizerDecoder
{
  public:
    ORMotionNodeDecoder();
    virtual ~ORMotionNodeDecoder() {}
    enum EMotionNodeConsts {kBufHeadLen = 3};
    
                                       
    virtual std::string GetDataObjectPath() { return "ORMotionNodeModel:XYZTrace"; }  
    virtual std::string GetDictionaryObjectPath() { return "ORMotionNodeModel"; }  
    virtual bool SetDataRecord(UInt_t* record);
       
    //Functions that return data from buffer header:
    virtual inline UShort_t GetChannelNum();
    virtual inline UInt_t   GetTimeStamp();
	virtual inline UShort_t Device();
	
    // Waveform Functions
    virtual inline UInt_t GetRecordOffset();
    virtual inline size_t GetWaveformLength(); 
    virtual inline UInt_t* GetWaveformDataPointer();
    virtual size_t CopyWaveformData(UInt_t* waveform, size_t len);
 
    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
    
    /* Adhering to ORVDigitizerDecoder interface. */
    virtual double GetSamplingFrequency();
    virtual UShort_t GetBitResolution() {return 32;}
    virtual size_t GetNumberOfEvents() {return 1;}
    virtual ULong64_t GetEventTime(size_t /*event*/)		{ return GetTimeStamp(); }
    virtual UInt_t GetEventEnergy(size_t /*event*/)			{ return 0; }
    virtual UShort_t GetEventChannel(size_t /*event*/)		{ return GetChannelNum();}
    virtual size_t GetEventWaveformLength(size_t /*event*/) { return GetWaveformLength(); }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, 
										  size_t waveformPoint );

};

//inline functions: ************************************************************************

inline UInt_t ORMotionNodeDecoder::GetRecordOffset()
{
	return (UShort_t) (kBufHeadLen);
}

inline UShort_t ORMotionNodeDecoder::Device()
{
	return (UShort_t) (fDataRecord[1] & 0x0000ffff);
}

inline UShort_t ORMotionNodeDecoder::GetChannelNum()
{
  return (UShort_t) ((fDataRecord[1] & 0x00030000) >> 16);
}

inline UInt_t ORMotionNodeDecoder::GetTimeStamp()
{
  return (fDataRecord[2]);
}

inline size_t ORMotionNodeDecoder::GetWaveformLength()
{
  return (LengthOf(fDataRecord) - kBufHeadLen); //in longs
}

inline UInt_t* ORMotionNodeDecoder::GetWaveformDataPointer()
{
  return (UInt_t*)(fDataRecord + kBufHeadLen);
}
#endif

