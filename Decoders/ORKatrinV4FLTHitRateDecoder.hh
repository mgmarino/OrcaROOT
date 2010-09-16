// ORKatrinV4FLTHitRateDecoder.hh

#ifndef _ORKatrinV4FLTHitRateDecoder_hh_
#define _ORKatrinV4FLTHitRateDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

/** Decodes the binary Orca data format and writes it into a ROOT TFile.
 * The binary data format description is in \file ORKatrinV4FLTDecoder.m .
 * In  \file ORKatrinV4FLTModel.m in in - (NSDictionary*) dataRecordDescription
 * the entries in the data dictionary define the data key and its according
 * selector of the decoder. In this case it is "KatrinV4FLT". The decoder of
 * this dictionary is defined as ORKatrinV4FLTDecoderForHitRate.
 * The source code (in \file ORKatrinV4FLTDecoder.m) of this method (ORKatrinV4FLTDecoderForHitRate)
 * holds the description of this format.
 *
 * This format is recognized by the return value of GetDataObjectPath() which is
 * "ORKatrinV4FLTModel:KatrinV4FLT".
 */
class ORKatrinV4FLTHitRateDecoder : public ORVBasicTreeDecoder
{
public:
  ORKatrinV4FLTHitRateDecoder() {}
  virtual ~ORKatrinV4FLTHitRateDecoder() {}

  enum EKatrinV4FLTHitRateConsts {kNumFLTChannels = 24,
                                  kHeaderSize     = 5 };
  virtual inline UShort_t NChannelsOf(UInt_t* record)
  { return ((UInt_t)(record[0] & 0x3FFFF)) - ((UInt_t)kHeaderSize); }
  virtual inline UInt_t   SecondsOf(UInt_t* record) { return record[2]; }
  virtual inline UInt_t   HitRateLengthOf(UInt_t* record) { return record[3]; }
  virtual inline UInt_t   TotalHitRateOf(UInt_t* record) { return record[4]; }
  virtual inline UShort_t ChannelOf(UInt_t i,UInt_t* record)
  { return ( record[i+kHeaderSize] & 0xFF00000 ) >> 20; }
  virtual inline Bool_t   OverflowOf(UInt_t i,UInt_t* record)
  { return ( record[i+kHeaderSize] & 0x10000 ) >> 16; }
  virtual inline UShort_t HitRateOf(UInt_t i,UInt_t* record)
  { return ( record[i+kHeaderSize] & 0xFFFF ); }

  // for basic trees
  virtual size_t GetNPars() { return 6; }
  virtual std::string GetParName(size_t iPar);
  virtual size_t GetNRows(UInt_t* /*record*/) { return 1; }
  virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

  virtual std::string GetDataObjectPath() { return "ORKatrinV4FLTModel:KatrinV4FLTHitRate"; }
  //!< KatrinV4FLT is the key in ORKatrinV4FLTModel.m in - (NSDictionary*) dataRecordDescription -tb- 2008-02-12

};


#endif
