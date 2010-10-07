// ORCaen1785adcDecoder.hh

#ifndef _ORCaen1785adcDecoder_hh_
#define _ORCaen1785adcDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORCaen1785adcDecoder : public ORVBasicTreeDecoder
{
  public:
    ORCaen1785adcDecoder();
    virtual ~ORCaen1785adcDecoder() {}

    virtual inline UInt_t IthChannelOf(UInt_t* record, size_t iRow)
      { return (record[iRow + 2] & 0x001c0000) >> 18; }
    virtual inline UInt_t IthValueOf(UInt_t* record, size_t iRow)
      { return record[iRow + 2] & 0x00000fff; }
    virtual inline UInt_t IthValueIsUnderThreshold(UInt_t* record, size_t iRow)
      { return (record[iRow + 2] & 0x00002000) >> 13; }
    virtual inline UInt_t IthValueIsOverflow(UInt_t* record, size_t iRow)
      { return (record[iRow + 2] & 0x00001000) >> 12; }
    virtual inline UInt_t IthValueIsValid(UInt_t* record, size_t iRow)
      { return ((record[iRow + 2] & 0x07000000) >> 24) == 0x000; }
    virtual inline UInt_t IthValueIsLowRange(UInt_t* record, size_t iRow)
      { return (record[iRow + 2] & 0x00020000) >> 17; }

    virtual std::string GetDataObjectPath() { return "ORCaen1785Model:Adc"; }
    virtual std::string GetValueName() { return "adc"; }

    // for basic trees
    virtual size_t GetNPars() { return 8; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* record) { return LengthOf(record) - 2; }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);
};

#endif
