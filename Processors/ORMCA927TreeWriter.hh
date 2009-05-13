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
    enum ORMCA927TreeWriterConsts{kMaxWFLength = 0x3fff};
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
	{ fDevice = 0;  fType = 0; fChannel = 0; 
        fWaveformLength = 0;}
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORMCA927Decoder* fEventDecoder;
    UShort_t fDevice, fChannel, fType;
	Double_t fLiveTime,fRealTime;
    UInt_t fWaveform[kMaxWFLength];
    UInt_t fZDT[kMaxWFLength];
    size_t fWaveformLength;
};

#endif
