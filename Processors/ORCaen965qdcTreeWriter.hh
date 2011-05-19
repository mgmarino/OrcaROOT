// ORCaen965qdcTreeWriter.hh

#ifndef __ORCaen965qdcTreeWriter_hh__
#define __ORCaen965qdcTreeWriter_hh__

#include "ORVTreeWriter.hh"
#include "ORCaen965qdcDecoder.hh"

class ORCaen965qdcTreeWriter : public ORVTreeWriter
{
  public:
    ORCaen965qdcTreeWriter(std::string treeName = "");
    virtual ~ORCaen965qdcTreeWriter();
    
    enum ORCaen965qdcTreeWriterConsts{kMaxSpLength = 4095, fSizeMax = 16};

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
	void Dump(UInt_t* record, size_t iRow);
  
    virtual inline void Clear() 
		{	
		fQuestionableEvent = 0;
		fSize 			   = 0;
		}
		
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORCaen965qdcDecoder* fEventDecoder;
	UInt_t fQuestionableEvent, fSize;
    UInt_t fQDCVal[fSizeMax],fChannel[fSizeMax],fRangeBit[fSizeMax];
    UInt_t fIsUnderThreshold[fSizeMax], fIsOverflow[fSizeMax];
    UInt_t fIsValidData[fSizeMax];
};

#endif
