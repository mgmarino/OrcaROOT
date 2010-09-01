// ORMCA927TreeWriter.hh

#ifndef __ORMCA927TreeWriter_hh__
#define __ORMCA927TreeWriter_hh__

#include "ORVTreeWriter.hh"
#include "ORMCA927Decoder.hh"

class ORMCA927TreeWriter : public ORVTreeWriter
{
  public:
    ORMCA927TreeWriter(std::string treeName = "");
    virtual ~ORMCA927TreeWriter();
    // changed to 0x4000 01 sept 2010, A. Schubert
    enum ORMCA927TreeWriterConsts{kMaxSpLength = 0x4000};
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
		{	
			fDevice			= 0;  
			fType			= 0; 
			fZDTMode		= 0; 
			fChannel		= 0; 
			fSpectrumLength = 0;
		}
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORMCA927Decoder* fEventDecoder;
    UShort_t fDevice, fChannel, fType;
	Double_t fLiveTime,fRealTime;
	UInt_t fZDTMode;
    UInt_t fSpectrum[kMaxSpLength];
    size_t fSpectrumLength;
};

#endif
