// ORKatrinFLTEnergyHistogramDecoder.hh

#ifndef _ORKatrinFLTEnergyHistogramDecoder_hh_
#define _ORKatrinFLTEnergyHistogramDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

/** Decodes the binary Orca data format and writes it into a ROOT TFile.
  * The binary data format description is in \file ORKatrinFLTDecoder.m .
  * In  \file ORKatrinFLTModel.m in in - (NSDictionary*) dataRecordDescription
  * the entries in the data dictionary define the data key and its according
  * selector of the decoder. In this case it is "KatrinFLTHistogram". The decoder of
  * this dictionary is defined as ORKatrinFLTDecoderForHistogram.
  * The source code (in \file ORKatrinFLTDecoder.m) of this method (ORKatrinFLTDecoderForHistogram)
  * holds the description of this format.
  *
  * This format is recognized by the return value of GetDataObjectPath() which is
  * "ORKatrinFLTModel:KatrinFLTHistogram".
  */ //-tb- 2008-07-25 .
class ORKatrinFLTEnergyHistogramDecoder : public ORVBasicTreeDecoder
{
  public:
    ORKatrinFLTEnergyHistogramDecoder() {fDataRecord = NULL; fHistogramLength = -1;}
    virtual ~ORKatrinFLTEnergyHistogramDecoder() {}

    enum EKatrinFLTEnergyConsts {kBufHeadLen = 10,
                                 kNumFLTChannels = 22};
    size_t fHistogramLength;
    
    virtual bool SetDataRecord(UInt_t* dataRecord);
    
    virtual inline UInt_t ChannelOf(UInt_t* record)
      { return ( record[1] & 0x00000ff0 ) >> 8; }
      
    //Functions that return data from buffer header:
    virtual inline UInt_t  ReadoutSecOf(UInt_t* record)  { return record[2]; }
    virtual inline UInt_t  RefreshTimeOf(UInt_t* record) { return record[3]; }
    virtual inline UInt_t  FirstBinOf(UInt_t* record)    { return record[4]; }
    virtual inline UInt_t  LastBinOf(UInt_t* record)     { return record[5]; }
    virtual inline UInt_t  HistogramLengthOf(UInt_t* record)    { return record[6]; }
    virtual inline UInt_t  MaxHistogramLengthOf(UInt_t* record) { return record[7]; }
    virtual inline UInt_t  BinSizeOf(UInt_t* record)     { return record[8]; }
    virtual inline UInt_t  OffsetEMinOf(UInt_t* record)  { return record[9]; }

    // Histogram Functions
    inline UInt_t* GetHistogramDataPointer();
    virtual size_t CopyHistogramData(UInt_t* histogram, size_t len);

    // for basic trees
    virtual size_t GetNPars() { return 11; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* /*record*/) { return 1; }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    virtual std::string GetDataObjectPath() { return "ORKatrinFLTModel:KatrinFLTHistogram"; }
    //!< KatrinFLTHistogram is the key in ORKatrinFLTModel.m in - (NSDictionary*) dataRecordDescription -tb- 2008-07-25

  protected:
    UInt_t* fDataRecord;

};

//inline functions: ************************************************************************
inline UInt_t* ORKatrinFLTEnergyHistogramDecoder::GetHistogramDataPointer()
{
  return (fDataRecord + kBufHeadLen);
}


#endif
