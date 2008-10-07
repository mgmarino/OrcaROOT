#ifndef _ORVDigitizerDecoder_hh_
#define _ORVDigitizerDecoder_hh_
#include <string>
#include "TROOT.h"
#include "ORVDataDecoder.hh"

//! Defines an interface for Digitizer decoders.
class ORVDigitizerDecoder: public ORVDataDecoder
{
  public:
    ORVDigitizerDecoder() {}
    virtual ~ORVDigitizerDecoder() {}

    /* Basic functions. */

    //! Should return in units of 1 GHz. 
    virtual double GetSamplingFrequency() = 0;
    virtual UShort_t GetBitResolution() = 0;
    virtual bool SetDataRecord(UInt_t* dataRecord) = 0;
    virtual inline UInt_t CrateOf();
    virtual inline UInt_t CardOf();

    /* Event Functions */

    //! It is possible to have more than one event in a record. 
    virtual size_t GetNumberOfEvents() = 0;
    virtual ULong64_t GetEventTime(size_t event) = 0;
    virtual UInt_t GetEventEnergy(size_t event) = 0; 
    virtual UShort_t GetEventChannel(size_t event) = 0;

    /* This is an optional overload that allows us to pass flags on to a root tree.*/
    /* The actual decoding of this UInt_t is left up to the user. */
    virtual UInt_t GetEventFlags(size_t /*event*/) { return 0; }
     
    /* Now waveforms */ 

    //! This length should be in number of words.
    virtual size_t GetEventWaveformLength(size_t event) = 0;

    //! Gets a point in the waveform.
    /**
       One can mask this this off to reduce the 
       size of the data using GetBitResolution(). 
       e.g. 
       \verbatim
           UInt_t mask = 0x1; size_t i=1; 
           while (i<GetBitResolution()) { mask <<= 1; mask += 1; i++; }  
       \endverbatim
    */
    virtual UInt_t GetEventWaveformPoint( size_t event, size_t waveformPoint ) = 0;

  protected:
    UInt_t* fDataRecord;
    

};

inline UInt_t ORVDigitizerDecoder::CrateOf() 
{ 
  return (fDataRecord[1] & 0x01e00000) >> 21; 
}

inline UInt_t ORVDigitizerDecoder::CardOf()
{ 
  return (fDataRecord[1] & 0x001f0000) >> 16; 
}


#endif
