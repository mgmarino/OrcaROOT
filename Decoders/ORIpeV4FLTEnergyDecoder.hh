// ORIpeV4FLTEnergyDecoder.hh

#ifndef _ORIpeV4FLTEnergyDecoder_hh_
#define _ORIpeV4FLTEnergyDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

/** Decodes the binary Orca data format and writes it into a ROOT TFile.
  * The binary data format description is in \file ORIpeV4FLTDecoder.m .
  * In  \file ORIpeV4FLTModel.m in in - (NSDictionary*) dataRecordDescription
  * the entries in the data dictionary define the data key and its according
  * selector of the decoder. In this case it is "IpeV4FLT". The decoder of
  * this dictionary is defined as ORIpeV4FLTDecoderForEnergy.
  * The source code (in \file ORIpeV4FLTDecoder.m) of this method (ORIpeV4FLTDecoderForEnergy)
  * holds the description of this format.
  *
  * This format is recognized by the return value of GetDataObjectPath() which is
  * "ORIpeV4FLTModel:IpeV4FLT".
  */ //-tb- 2008-02-6 .
class ORIpeV4FLTEnergyDecoder : public ORVBasicTreeDecoder
{
  public:
    ORIpeV4FLTEnergyDecoder() {}
    virtual ~ORIpeV4FLTEnergyDecoder() {}

    enum EIpeV4FLTEnergyConsts {kNumFLTChannels = 24};
    virtual inline UShort_t ChannelOf(UInt_t* record)
      { return ( record[1] & 0xFF00 ) >> 8; }
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

    virtual std::string GetDataObjectPath() { return "ORIpeV4FLTModel:IpeV4FLTEnergy"; }
    //!< IpeV4FLT is the key in ORIpeV4FLTModel.m in - (NSDictionary*) dataRecordDescription -tb- 2008-02-12

};


#endif
