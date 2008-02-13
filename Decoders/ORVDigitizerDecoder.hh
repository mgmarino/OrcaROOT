#ifndef _ORVDigitizerDecoder_hh_
#define _ORVDigitizerDecoder_hh_
#include <string>
#include "TROOT.h"
#include "ORVDataDecoder.hh"

class ORVDigitizerDecoder: public ORVDataDecoder
{
  public:
    ORVDigitizerDecoder() {}
    virtual ~ORVDigitizerDecoder() {}

    /* Basic functions. */
    virtual double GetSamplingFrequency() = 0;
      /* Should return in units of 1 GHz. */
    virtual inline UShort_t GetSizeOfWaveformDatum() { return sizeof(UShort_t); }
      /* This is the size of one word (in bytes) of waveform data.  
         Default to 16-bit.*/
    virtual inline UShort_t GetBitResolution() = 0;
    virtual bool SetDataRecord(UInt_t* dataRecord) = 0;
    virtual inline UInt_t CrateOf();
    virtual inline UInt_t CardOf();

    /* Event Functions */
    virtual size_t GetNumberOfEvents() = 0;
      /* It is possible to have more than one event in a record. */
    virtual ULong64_t GetEventTime(size_t event) = 0;
    virtual UInt_t GetEventEnergy(size_t event) = 0; 
    virtual UShort_t GetEventChannel(size_t event) = 0;
      
     /* Now waveforms */ 
     virtual size_t GetEventWaveformLength(size_t event) = 0;
       /* This length should be in number of words, NOT bytes. */
       /* Word size is given above by GetSizeOfWaveformDatum() */
     virtual void* GetEventWaveformPointer(size_t event) = 0;
       /* Passes a void*, get the size of the data using 
          GetSizeOfWaveformDatum() and GetEventWaveformLength(event) */

  protected:
    UInt_t* fDataRecord;
    

};
inline UInt_t ORVDigitizerDecoder::CrateOf() //returns crate # of Greta card
{ 
  return (fDataRecord[1] & 0x01e00000) >> 21; 
}

inline UInt_t ORVDigitizerDecoder::CardOf()
{ 
  return (fDataRecord[1] & 0x001f0000) >> 16; 
}


#endif
