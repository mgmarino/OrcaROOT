// ORSIS3820Decoder.hh

#ifndef _ORSIS3820Decoder_hh_
#define _ORSIS3820Decoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORSIS3820Decoder : public ORVBasicTreeDecoder
{
public:
  ORSIS3820Decoder() {}
  virtual ~ORSIS3820Decoder() {}

  virtual inline UInt_t CrateOf(UInt_t* record)
  { return (record[1] & 0x1e00000) >> 21; }
  virtual inline UInt_t CardOf(UInt_t* record)
  { return (record[1] & 0x1f0000) >> 16; }
  virtual inline size_t NScalersOf(UInt_t* /*record*/)
  { return 32; }
  virtual inline UInt_t TimeRead(UInt_t* record)
  { return record[2]; }
  virtual inline UInt_t LastTimeRead(UInt_t* record)
  { return record[3]; }
  virtual inline UInt_t CountEnabledMask(UInt_t* record)
  { return record[4]; }
  virtual inline UInt_t OverflowMask(UInt_t* record)
  { return record[5]; }
  virtual inline UInt_t Options(UInt_t* record)
  { return record[6]; }
  virtual inline UInt_t IthChannelOf(UInt_t* /*record*/, size_t i) 
  { return i+1; }
  virtual inline UInt_t IthScalerOf(UInt_t* record, size_t i) 
  { return record[7+i]; }

  virtual std::string GetDataObjectPath() { return "ORSIS3820Model:Counts"; }

  // for basic trees
  virtual size_t GetNPars() { return 4; }
  virtual std::string GetParName(size_t iPar);
  virtual size_t GetNRows(UInt_t* record) { return NScalersOf(record); }
  virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);
};

#endif
