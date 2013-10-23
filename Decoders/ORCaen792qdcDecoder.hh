// ORCaen792qdcDecoder.hh

#ifndef _ORCaen792qdcDecoder_hh_
#define _ORCaen792qdcDecoder_hh_

#include <vector>
#include "ORVBasicTreeDecoder.hh"

class ORCaen792qdcDecoder : public ORVBasicTreeDecoder
{
  public:
    ORCaen792qdcDecoder();
    virtual ~ORCaen792qdcDecoder() {}

    virtual size_t EventCountOf(UInt_t* record) 
      { return record[LengthOf(record)-1] & 0xffffff; }
    virtual size_t NValuesOf(UInt_t* record);
    virtual inline UInt_t IthChannelOf(UInt_t* record, size_t iRow)
      { return (GetLocPtr(record, iRow)[0] & 0x003f0000) >> 16; }
    virtual inline UInt_t IthValueOf(UInt_t* record, size_t iRow)
      { return GetLocPtr(record, iRow)[0] & 0x00000fff; }
    virtual inline UInt_t IthValueIsUnderThreshold(UInt_t* record, size_t iRow)
      { return (GetLocPtr(record, iRow)[0] & 0x00002000) >> 13; }
    virtual inline UInt_t IthValueIsOverflow(UInt_t* record, size_t iRow)
      { return (GetLocPtr(record, iRow)[0] & 0x00001000) >> 12; }
    virtual inline UInt_t IthValueIsValid(UInt_t* record, size_t iRow)
      { return ((GetLocPtr(record, iRow)[0] & 0x07000000) >> 24) == 0x000; }

    virtual std::string GetDataObjectPath() { return "ORCaen792Model:Qdc"; }
    virtual std::string GetValueName() { return "qdc"; }
    virtual size_t GetNChannels() { return 32; }

    // for basic trees
    virtual size_t GetNPars() { return 7; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* record) { return NValuesOf(record); }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);

  protected:
    virtual void LoadLocPtrs(UInt_t* record);
    virtual UInt_t* GetLocPtr(UInt_t* record, size_t i);

    virtual inline UInt_t IthWordIsHeader(UInt_t* record, size_t iWord)
      { return ((record[iWord] & 0x07000000) >> 24) == 2; }
    virtual inline size_t NChannelsInBlock(UInt_t* headerPtr)
      { return (headerPtr[0] & 0x00003f00) >> 8; }

    virtual inline UInt_t IthWordIsData(UInt_t* record, size_t iWord)
      { return ((record[iWord] & 0x07000000) >> 24) == 0; }
    virtual inline UInt_t IthWordIsInvalidData(UInt_t* record, size_t iWord)
      { return ((record[iWord] & 0x07000000) >> 24) == 6; }

    virtual inline UInt_t IthWordIsEndOfBlock(UInt_t* record, size_t iWord)
      { return ((record[iWord] & 0x07000000) >> 24) == 4; }

    std::vector<UInt_t*> fLocPtrs;
    UInt_t fLastEventCount;
};

#endif
