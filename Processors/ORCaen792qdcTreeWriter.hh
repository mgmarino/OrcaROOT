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
    
    virtual inline void Clear();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORCaen792qdcDecoder* fEventDecoder;
    UInt_t fCrate;
    UInt_t fCard;
    UInt_t fEventCount;
    UInt_t fNValues;
    std::vector<UInt_t> fQDCVal;
    std::vector<UInt_t> fIsUnderThreshold;
    std::vector<UInt_t> fIsOverflow;
};

#endif
