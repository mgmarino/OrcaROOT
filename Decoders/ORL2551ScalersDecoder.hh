// ORL2551ScalersDecoder.hh

#ifndef _ORL2551ScalersDecoder_hh_
#define _ORL2551ScalersDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORL2551ScalersDecoder : public ORVBasicTreeDecoder
{
  public:
    ORL2551ScalersDecoder() {}
    virtual ~ORL2551ScalersDecoder() {}

    virtual inline UInt_t CrateOf(UInt_t* record)
      { return (record[1] & 0x000f0000) >> 16; }
    virtual inline UInt_t CardOf(UInt_t* record)
      { return (record[1] & 0x0000001f); }
    virtual inline size_t NScalersOf(UInt_t* /*record*/)
      { return 12; }
    virtual inline UInt_t IthChannelOf(UInt_t* record, size_t i) 
      { return (record[2+i] & 0xf0000000) >> 28; }
    virtual inline UInt_t IthScalerOf(UInt_t* record, size_t i) 
      { return (record[2+i] & 0x00ffffff); }

    virtual std::string GetDataObjectPath() { return "L2551:Scalers"; }

    // for basic trees
    virtual size_t GetNPars() { return 4; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* record) { return NScalersOf(record); }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);
};

#endif
