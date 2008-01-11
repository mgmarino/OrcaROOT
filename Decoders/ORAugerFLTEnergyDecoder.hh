// ORAugerFLTEnergyDecoder.hh

#ifndef _ORAugerFLTEnergyDecoder_hh_
#define _ORAugerFLTEnergyDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORAugerFLTEnergyDecoder : public ORVBasicTreeDecoder
{
  public:
    ORAugerFLTEnergyDecoder() {}
    virtual ~ORAugerFLTEnergyDecoder() {}

    enum EAugerFLTEnergyConsts {kNumFLTChannels = 22};
    virtual inline UShort_t ChannelOf(UInt_t* record);
    virtual inline UInt_t SecondsOf(UInt_t* record) { return record[2]; }
    virtual inline UInt_t SubSecondsOf(UInt_t* record) { return record[3]; }
    virtual inline UInt_t ChannelMapOf(UInt_t* record) { return record[4]; }
    virtual inline UInt_t EventIDOf(UInt_t* record) { return record[5]; }
    virtual inline UInt_t EnergyOf(UInt_t* record) { return record[6]; }

    // for basic trees
    virtual size_t GetNPars() { return 6; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* /*record*/) { return 1; }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    virtual std::string GetDataObjectPath() { return "ORAugerFLTModel:AugerFLT"; }

};

inline UShort_t ORAugerFLTEnergyDecoder::ChannelOf(UInt_t* record)
{
  for(size_t i=0; i<kNumFLTChannels; i++) {
    if((0x00000001<<i) & ChannelMapOf(record)) return i; 
  }
  return (UShort_t)(-1);
  /* If we get here, there's an error*/
}
#endif
