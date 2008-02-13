// ORKatrinFLTEnergyDecoder.hh

#ifndef _ORKatrinFLTEnergyDecoder_hh_
#define _ORKatrinFLTEnergyDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORKatrinFLTEnergyDecoder : public ORVBasicTreeDecoder
{
  public:
    ORKatrinFLTEnergyDecoder() {}
    virtual ~ORKatrinFLTEnergyDecoder() {}

    enum EKatrinFLTEnergyConsts {kNumFLTChannels = 22};
    virtual inline UShort_t ChannelOf(UInt_t* record)
      { return ( record[4] & 0xFF000000 ) >> 24; }
    virtual inline UInt_t SecondsOf(UInt_t* record) { return record[2]; }
    virtual inline UInt_t SubSecondsOf(UInt_t* record) { return record[3]; }
    virtual inline UInt_t ChannelMapOf(UInt_t* record) 
      { return (record[4] & 0x3FFFFF); }
    virtual inline UInt_t EventIDOf(UInt_t* record) 
      { return ( record[5] & 0x3FF ); }
    virtual inline UInt_t PageNumberOf(UInt_t* record) 
      { return ( record[5] & 0x1FF0000 ) >> 16; }
    virtual inline UInt_t EnergyOf(UInt_t* record) { return record[6]; }
    virtual inline UInt_t ResetSecondsOf(UInt_t* record) { return record[7]; }
    virtual inline UInt_t ResetSubSecondsOf(UInt_t* record) { return record[8]; }

    // for basic trees
    virtual size_t GetNPars() { return 6; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* /*record*/) { return 1; }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    virtual std::string GetDataObjectPath() { return "ORKatrinFLTModel:KatrinFLT"; }

};


#endif
