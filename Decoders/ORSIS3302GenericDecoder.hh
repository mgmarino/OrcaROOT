// ORSIS3302GenericDecoder.hh

#ifndef _ORSIS3302GenericDecoder_hh_
#define _ORSIS3302GenericDecoder_hh_

#include "ORVDigitizerDecoder.hh"

class ORSIS3302GenericDecoder: public ORVDigitizerDecoder
{
public:
    ORSIS3302GenericDecoder();
    virtual ~ORSIS3302GenericDecoder() {}
    
    enum ESIS3302GenericConsts { kOrcaHeaderLen = 4,
		};
	
    virtual std::string GetDataObjectPath() { return "ORSIS3302Generic:Waveform"; }  
    virtual std::string GetDictionaryObjectPath() { return "ORSIS3302GenericModel"; }
    virtual bool SetDataRecord(UInt_t* record);
	
    //-------------------------------------------------------------
    virtual inline UInt_t GetWaveformTag();
    virtual inline UInt_t GetTotalWaveformLength();
	
    // Returns maximum energy found in the gate
	
    // Waveform Functions
	
    // Waveform length in number of 16-bit words
    virtual inline size_t GetWaveformLen() { return (LengthOf(fDataRecord)-kOrcaHeaderLen)*2; }
    virtual size_t CopyWaveformData(UShort_t* waveform, size_t len);
    virtual size_t CopyWaveformDataDouble(double* waveform, size_t len);
    virtual inline UInt_t* GetWaveformDataPointer();
	
    // Energy Waveform length in number of 32-bit words
	
    /* Functions that return information about card/channel settings. */
    /* These are static throughout a run, so a processor should take  * 
     * advantage of this and maybe not query during each record.      */
	
    /* Functions satisfying the ORVDigitizerDecoder interface. */
    virtual inline double GetSamplingFrequency();
    virtual inline UShort_t GetBitResolution() { return 16; }
    virtual inline size_t GetNumberOfEvents() { return 1; }
    virtual inline ULong64_t GetEventTime(size_t /*event*/) 
	{ return 0; }
    virtual inline UInt_t GetEventEnergy(size_t /*event*/) 
	{ return 0; }
    virtual inline UShort_t GetEventChannel(size_t /*event*/) 
	{ return (fDataRecord[1] & 0xFF00) >> 8; }
    virtual size_t GetEventWaveformLength(size_t /*event*/) 
	{ return GetWaveformLen(); }
    virtual UInt_t GetEventWaveformPoint( size_t /*event*/, 
					 size_t waveformPoint );
    
    virtual UInt_t GetEventFlags(size_t /*event*/) { return 0; }
    
    // added 06 Aug 2009, A. Schubert
    virtual Bool_t WaveformDataIsSigned() { return false; }
	
    //Error checking:
    virtual bool IsValid();
	
    //debugging:
    void Dump(UInt_t* dataRecord);

    enum EClockType
    {
      k100MHzBad = 0,
      k50MHz,
      k25MHz,
      k10MHz,
      k1MHz,
      kExternalRand,
      kExternal,
      k100MHz
    };
    
    UInt_t GetAveragingForChannel(size_t chan); 
    EClockType GetClockType(); 
protected:
    /* GetRecordOffset() returns how many words the record is offset from the 
	 beginning.  This is useful when additional headers are added. */
    virtual inline size_t GetRecordOffset() {return kOrcaHeaderLen;}

};

//inline functions: ************************************************************************

inline UInt_t ORSIS3302GenericDecoder::GetWaveformTag()
{
	return fDataRecord[2];
}

inline UInt_t ORSIS3302GenericDecoder::GetTotalWaveformLength()
{
	return fDataRecord[3];
}


inline UInt_t* ORSIS3302GenericDecoder::GetWaveformDataPointer()
{
	return (fDataRecord + GetRecordOffset());
}

#endif

