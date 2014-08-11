// ORCaen792qdcMultiCardTreeWriter.hh

#ifndef __ORCaen792qdcMultiCardTreeWriter_hh__
#define __ORCaen792qdcMultiCardTreeWriter_hh__

#include "ORVTreeWriter.hh"
#include "ORCaen792qdcDecoder.hh"

class ORCaen792qdcMultiCardTreeWriter : public ORVTreeWriter
{
  public:
    enum E792Model { k792A, k792N };
    ORCaen792qdcMultiCardTreeWriter(E792Model model = k792A, std::string treeName = "");
    virtual ~ORCaen792qdcMultiCardTreeWriter();
    
    virtual inline void Clear();
    virtual EReturnCode BeginRun();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual EReturnCode EndRun();

  protected:
    virtual EReturnCode InitializeBranches();

  protected:
    ORCaen792qdcDecoder* fEventDecoder;
    std::vector<UInt_t> fCrate;
    std::vector<UInt_t> fCard;
    UInt_t fEventCount;
    UInt_t fNValues;

    std::vector<UInt_t> fQDCVal;
    std::vector<UInt_t> fIsUnderThreshold;
    std::vector<UInt_t> fIsOverflow;
};

#endif
