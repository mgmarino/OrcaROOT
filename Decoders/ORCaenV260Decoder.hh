// ORCaenV260Decoder.hh

#ifndef _ORCaenV260Decoder_hh_
#define _ORCaenV260Decoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORCaenV260Decoder : public ORVBasicTreeDecoder
{
  public:
     
    ORCaenV260Decoder() {}
    virtual ~ORCaenV260Decoder() {}

    virtual inline UInt_t CrateOf(UInt_t* record)
      { return (record[1] & 0x001E00000) >> 21; }
    virtual inline UInt_t CardOf(UInt_t* record)
      { return (record[1] & 0x00001F0000) >> 16; }
    virtual inline UInt_t UTimeOf(UInt_t* record)
      { return record[2]; }
    virtual inline size_t NScalersOf(UInt_t* /*record*/)
      { return 16; }
    virtual inline UInt_t IthChannelOf(size_t i) 
      { return i; }
    virtual inline UInt_t IthScalerOf(UInt_t* record, size_t i) 
      { return (record[3+i] & 0x00ffffff); }

    virtual inline std::string GetLabel() { return "CaenV260"; }
    virtual inline std::string GetDataObjectPath() 
      { return "ORCaen260Model:Caen260"; }

    // for basic trees
    virtual size_t GetNPars() { return 5; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* record) { return NScalersOf(record); }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);
};

#endif
