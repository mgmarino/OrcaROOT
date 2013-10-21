// ORCaen792qdcTreeWriter.hh

#ifndef __ORCaen792qdcTreeWriter_hh__
#define __ORCaen792qdcTreeWriter_hh__

#include "ORVTreeWriter.hh"
#include "ORCaen792qdcDecoder.hh"

class ORCaen792qdcTreeWriter : public ORVTreeWriter
{
  public:
    enum E792Model { k792A, k792N };
    ORCaen792qdcTreeWriter(E792Model model = k792A, std::string treeName = "");
    virtual ~ORCaen792qdcTreeWriter();
    
    enum ORCaen792qdcTreeWriterConsts{kMaxSpLength = 4095, fSizeMax = 16};

    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
	void Dump(UInt_t* record, size_t iRow);
  
    virtual inline void Clear();
		
  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORCaen792qdcDecoder* fEventDecoder;
    UInt_t fCrate;
    UInt_t fCard;
    std::vector<UInt_t> fChannel;
    std::vector<UInt_t> fQDCVal;
    std::vector<UInt_t> fIsUnderThreshold;
    std::vector<UInt_t> fIsOverflow;
    std::vector<UInt_t> fIsValidData;
};

#endif
