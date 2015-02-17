// OREdelweissSLTFLTEventDecoder.hh

#ifndef _OREdelweissSLTFLTEventDecoder_hh_
#define _OREdelweissSLTFLTEventDecoder_hh_

#include "ORVDigitizerDecoder.hh"

/** Decodes the binary Orca data format and writes it into a ROOT TFile.
 * The binary data format description is in \file OREdelweissSLTDecoder.m .
 * In  \file OREdelweissSLTModel.m in - (NSDictionary*) dataRecordDescription
 * the entries in the data dictionary define the data key and its according
 * selector of the decoder. In this case it is "EdelweissSLTFLTEvent". The decoder of
 * this dictionary is defined as OREdelweissSLTDecoderForWaceForm.
 * The source code (in \file OREdelweissSLTDecoder.m) of this method (OREdelweissSLTDecoderForWaveForm)
 * holds the description of this format.
 *
 * Note to the name: a FLTevent is read out by the SLT FIFO -> SLTFLTEvent
 *
 * This format is recognized by the return value of GetDataObjectPath() which is
 * "OREdelweissSLTModel:EdelweissSLTFLTEvent".
 */ //-tb- 2013-02-6
class OREdelweissSLTFLTEventDecoder: public ORVDigitizerDecoder
{
public:
    OREdelweissSLTFLTEventDecoder();
    virtual ~OREdelweissSLTFLTEventDecoder() {}
    enum EEdelweissSLTFLTEventConsts {kBufHeadLen = 9, 
		kWaveformLength = 10000, //this is no longer valid/constant, use GetWaveformLen() -tb-
		//maybe rename to kWaveformPageLength ... ?
		kNumFLTChannels = 24};
    size_t fWaveformLength;
    
    virtual std::string GetDataObjectPath() { return "OREdelweissSLTModel:EdelweissSLTFLTEvent"; }  
    //!< EdelweissSLTFLTEvent is the key in OREdelweissSLTModel.m in - (NSDictionary*) OREdelweissSLTModel::dataRecordDescription -tb- 2008-02-12 
	
    virtual bool SetDataRecord(UInt_t* record);
	
    //Functions that return data from buffer header:
    virtual inline UInt_t GetSec();
    virtual inline UInt_t GetSubSec();
    virtual inline UInt_t GetChannelMap();
    virtual inline UShort_t GetFiber();
    virtual inline UShort_t GetChannel();
    virtual inline UShort_t GetTrigChannel();
    virtual inline UInt_t GetEventInfo();
    virtual inline UInt_t GetEnergy();
    virtual inline UInt_t GetEventID();
    virtual inline UInt_t GetEventFlags(size_t event=0);
	
    // Waveform Functions
    virtual inline size_t GetWaveformLen(); 
    virtual inline UInt_t* GetWaveformDataPointer();
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);
    virtual size_t CopyWaveformData(UShort_t* waveform, size_t len);
	
    /* Satisfying ORVDigitizerDecoder interface. */
    virtual double GetSamplingFrequency() {return .01;}
    virtual UShort_t GetBitResolution() {return 16;} 
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

/*! Returns the number of the waveform in shorts (two shorts are stored in one long int / int32).
 *  Will be set by SetDataRecord(...)
 */ // -tb-
inline size_t OREdelweissSLTFLTEventDecoder::GetWaveformLen() 
{
	return fWaveformLength;
} 

inline UInt_t OREdelweissSLTFLTEventDecoder::GetSubSec() 
{
	return (fDataRecord[2]); //f0 is lsb of timestamp -tb-
}

inline UInt_t OREdelweissSLTFLTEventDecoder::GetSec()
{
	return (fDataRecord[3]); //f1 & 0xffff is msb of timestamp -tb-
}

inline UInt_t OREdelweissSLTFLTEventDecoder::GetChannelMap()
{
	return (fDataRecord[4]);//f2 = channel map (18 bit) -tb-
}

inline UInt_t OREdelweissSLTFLTEventDecoder::GetEnergy()
{
	return (fDataRecord[5] & 0x00FFFFFF);//f3: energy (and U(?)) -tb-
}

inline UInt_t OREdelweissSLTFLTEventDecoder::GetEventInfo() //changed  2011-06-14 -tb-
{
	return ( fDataRecord[6] );//f4: page + triggAddr
}

inline UInt_t OREdelweissSLTFLTEventDecoder::GetEventID()
{
	return ( ( fDataRecord[6] & 0x0000F000 ) >> 12);
}

inline UInt_t OREdelweissSLTFLTEventDecoder::GetEventFlags(size_t)
{
	return (fDataRecord[7]);//flags (WF version etc.)
}


//TODO: fDataRecord[8] = spare


inline UShort_t OREdelweissSLTFLTEventDecoder::GetFiber()
{
	return ( fDataRecord[1] & 0xF000 ) >> 12;
}

inline UShort_t OREdelweissSLTFLTEventDecoder::GetChannel()
{
	return ( fDataRecord[1] & 0x0F00 ) >> 8;
}

inline UShort_t OREdelweissSLTFLTEventDecoder::GetTrigChannel()
{
	return ( fDataRecord[1] & 0xFF00 ) >> 8;
}

inline UInt_t* OREdelweissSLTFLTEventDecoder::GetWaveformDataPointer()
{
	return (fDataRecord + kBufHeadLen);
}

inline ULong64_t OREdelweissSLTFLTEventDecoder::GetEventTime( size_t )
{
	return ((ULong64_t)GetSec() << 32 ) + (ULong64_t)GetSubSec();
}

#endif

