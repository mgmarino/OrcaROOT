// ORKatrinV4FLTWaveformDecoder.hh

#ifndef _ORKatrinV4FLTWaveformDecoder_hh_
#define _ORKatrinV4FLTWaveformDecoder_hh_

#include "ORVDigitizerDecoder.hh"

/** Decodes the binary Orca data format and writes it into a ROOT TFile.
 * The binary data format description is in \file ORKatrinV4FLTDecoder.m .
 * In  \file ORKatrinV4FLTModel.m in - (NSDictionary*) dataRecordDescription
 * the entries in the data dictionary define the data key and its according
 * selector of the decoder. In this case it is "KatrinV4FLTWaveForm". The decoder of
 * this dictionary is defined as ORKatrinV4FLTDecoderForEnergy.
 * The source code (in \file ORKatrinV4FLTDecoder.m) of this method (ORKatrinV4FLTDecoderForWaveForm)
 * holds the description of this format.
 *
 * This format is recognized by the return value of GetDataObjectPath() which is
 * "ORKatrinV4FLTModel:KatrinV4FLTWaveForm".
 */ //-tb- 2008-02-6
class ORKatrinV4FLTWaveformDecoder: public ORVDigitizerDecoder
{
public:
    ORKatrinV4FLTWaveformDecoder();
    virtual ~ORKatrinV4FLTWaveformDecoder() {}
    enum EKatrinV4FLTWaveformConsts {kBufHeadLen = 9, 
		kWaveformLength = 2048, //this is no longer valid, use GetWaveformLen() -tb-
		//maybe rename to kWaveformPageLength ... ?
		kNumFLTChannels = 24};
    size_t fWaveformLength;
    
    virtual std::string GetDataObjectPath() { return "ORKatrinV4FLTModel:KatrinV4FLTWaveForm"; }  
    //!< KatrinV4FLTWaveForm is the key in ORKatrinV4FLTModel.m in - (NSDictionary*) ORKatrinV4FLTModel::dataRecordDescription -tb- 2008-02-12 
	
    virtual bool SetDataRecord(UInt_t* record);
	
    //Functions that return data from buffer header:
    virtual inline UInt_t GetSec();
    virtual inline UInt_t GetSubSec();
    virtual inline UInt_t GetChannelMap();
    virtual inline UShort_t GetChannel();
    virtual inline UInt_t GetEventID();
    virtual inline UInt_t GetEnergy();
    virtual inline UInt_t GetEventFlags();
    virtual inline UInt_t GetEventInfo();
	
    // Waveform Functions
    virtual inline size_t GetWaveformLen(); 
    virtual inline UInt_t* GetWaveformDataPointer();
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);
    virtual size_t CopyWaveformData(UShort_t* waveform, size_t len);
	
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

/*! Returns the number of the waveform in shorts (two shorts are stored in one long int).
 *  Will be set by SetDataRecord(...)
 */ // -tb-
inline size_t ORKatrinV4FLTWaveformDecoder::GetWaveformLen() 
{
	return fWaveformLength;
} 

inline UInt_t ORKatrinV4FLTWaveformDecoder::GetSec()
{
	return (fDataRecord[2]);
}

inline UInt_t ORKatrinV4FLTWaveformDecoder::GetSubSec()
{
	return (fDataRecord[3]);
}

inline UInt_t ORKatrinV4FLTWaveformDecoder::GetChannelMap()
{
	return (fDataRecord[4]);
}

inline UInt_t ORKatrinV4FLTWaveformDecoder::GetEventID() //this holds now more info than only the page #, see ORKatrinV4FLTDecoder -tb-
{
	return ( fDataRecord[5] );
}

inline UInt_t ORKatrinV4FLTWaveformDecoder::GetEnergy()
{
	return (fDataRecord[6]);
}

inline UInt_t ORKatrinV4FLTWaveformDecoder::GetEventFlags()
{
	return (fDataRecord[7]);
}

inline UInt_t ORKatrinV4FLTWaveformDecoder::GetEventInfo()
{
	return (fDataRecord[8]);
}

inline UShort_t ORKatrinV4FLTWaveformDecoder::GetChannel()
{
	return ( fDataRecord[1] & 0xFF00 ) >> 8;
}

inline UInt_t* ORKatrinV4FLTWaveformDecoder::GetWaveformDataPointer()
{
	return (fDataRecord + kBufHeadLen);
}

inline ULong64_t ORKatrinV4FLTWaveformDecoder::GetEventTime( size_t )
{
	return ((ULong64_t)GetSec() << 32 ) + (ULong64_t)GetSubSec();
}

#endif

