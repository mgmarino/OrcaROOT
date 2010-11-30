// ORMotionNodeTreeWriter.hh

#ifndef __ORMotionNodeTreeWriter_hh__
#define __ORMotionNodeTreeWriter_hh__

#include "ORVTreeWriter.hh"
#include "ORMotionNodeDecoder.hh"

class ORMotionNodeTreeWriter : public ORVTreeWriter
{
  public:
    ORMotionNodeTreeWriter(std::string treeName = "");
    virtual ~ORMotionNodeTreeWriter();
    enum ORMotionNodeTreeWriterConsts{kMaxwLength = 1000};
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
		{	
			fDevice			= 0;  
			fChannel		= 0; 
			fWaveformLength = 0;
            fTimeStamp      = 0;
		}
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORMotionNodeDecoder* fEventDecoder;
    UShort_t fDevice, fChannel;
    UInt_t fWaveform[kMaxwLength], fTimeStamp;
    size_t fWaveformLength;
};

#endif
