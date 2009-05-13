// ORMCA927Decoder.hh

#ifndef _ORMCA927Decoder_hh_
#define _ORMCA927Decoder_hh_

#include "ORVDigitizerDecoder.hh"
/*----------------------------------------------
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
 ^^^^ ^^^^ ^^^^ ^^-----------------------data id
                  ^^ ^^^^ ^^^^ ^^^^ ^^^^-length in longs
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
                       ^-----------------zdt (1=ZDT, 0=Normal)
                        ^----------------channel
						  ^^^^ ^^^^ ^^^^-device
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx liveTime (20ms/bit, i.e. multiply by 0.02 to get seconds)
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx realTime (20ms/bit, i.e. multiply by 0.02 to get seconds)
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx 
                                 ^-------zdt Spectra enabled
                                  ^------zdt counting enabled
                                    ^^^^-zdt speed
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx spare
 the waveform follows and fills out the record 
 ------------------------------------------------*/
class ORMCA927Decoder: public ORVDigitizerDecoder
{
  public:
    ORMCA927Decoder();
    virtual ~ORMCA927Decoder() {}
    enum EMCA927Consts {kBufHeadLen = 10};
                                           
    virtual std::string GetDataObjectPath()			{ return "ORMCA927Model:Spectrum"; }  
    virtual std::string GetDictionaryObjectPath()	{ return "ORMCA927Model"; }  
    virtual bool SetDataRecord(UInt_t* record);
       
	//inline functions: ************************************************************************
	inline UShort_t GetType()				{ return ( fDataRecord[1] & 0x00002000 ) >> 13;}
	inline UShort_t GetChannelNum()			{ return ( fDataRecord[1] & 0x00001000 ) >> 12;}
	inline UShort_t GetDevice()				{ return ( fDataRecord[1] & 0x00000fff );}
	inline Double_t GetLiveTime()			{ return (fDataRecord[2] * 0.020);}
	inline Double_t GetRealTime()			{ return (fDataRecord[3] * 0.020);}
	inline Int_t    GetZDTMode()			{ return (fDataRecord[4]);}
	inline size_t   GetWaveformLen()		{ return (LengthOf(fDataRecord) - kBufHeadLen);}
	inline UInt_t*  GetWaveformDataPointer(){ return (UInt_t*)(fDataRecord + kBufHeadLen);}
	
    // Waveform Functions
    virtual size_t CopyWaveformData(UInt_t* waveform, size_t len);
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);
 	
    //Error checking:
    virtual bool IsValid();
    virtual void DumpBufferHeader();
   
    //debugging:
    void Dump(UInt_t* dataRecord);
    
    /* Adhering to ORVDigitizerDecoder interface. This device doesn't use most of these*/
    virtual double GetSamplingFrequency()					{ return 1;}
    virtual UShort_t GetBitResolution()						{ return 12;}
    virtual size_t GetNumberOfEvents()						{ return 1;}
    virtual ULong64_t GetEventTime(size_t /*event*/)		{ return 0; }
    virtual UInt_t GetEventEnergy(size_t /*event*/)			{ return 0; }
    virtual UShort_t GetEventChannel(size_t /*event*/)		{ return GetChannelNum();}
    virtual size_t GetEventWaveformLength(size_t /*event*/) { return GetWaveformLen(); }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, size_t waveformPoint );

};

#endif

