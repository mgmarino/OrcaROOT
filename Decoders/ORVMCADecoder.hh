#ifndef _ORVMCADecoder_hh_
#define _ORVMCADecoder_hh_
#include <string>
#include "ORVDataDecoder.hh"

//! Defines an interface for MultiChannel Analysers decoders.
class ORVMCADecoder: public ORVDataDecoder
{
  public:
    ORVMCADecoder() {}
    virtual ~ORVMCADecoder() {}

    //! This length should be in number of words.
    virtual size_t GetSpectrumLength() = 0;
 	virtual UInt_t	GetEventSpectrumPoint( size_t /*event*/, size_t spectrumPoint ) = 0;

  protected:
    UInt_t* fDataRecord;
    
};

#endif
