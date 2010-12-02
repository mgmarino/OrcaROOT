// ORKatrinV4FLTEnergyHistogramDecoder.hh

#ifndef _ORKatrinV4FLTEnergyHistogramDecoder_hh_
#define _ORKatrinV4FLTEnergyHistogramDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

/** Decodes the binary Orca data format and writes it into a ROOT TFile.
  * The binary data format description is in \file ORKatrinV4FLTDecoder.m .
  * In  \file ORKatrinV4FLTModel.m in in - (NSDictionary*) dataRecordDescription
  * the entries in the data dictionary define the data key and its according
  * selector of the decoder. In this case it is "KatrinV4FLTHistogram". The decoder of
  * this dictionary is defined as ORKatrinV4FLTDecoderForHistogram.
  * The source code (in \file ORKatrinV4FLTDecoder.m) of this method (ORKatrinV4FLTDecoderForHistogram)
  * holds the description of this format.
  *
  * This format is recognized by the return value of GetDataObjectPath() which is
  * "ORKatrinV4FLTModel:KatrinV4FLTHistogram".
  */ //-tb- 2008-07-25 .
class ORKatrinV4FLTEnergyHistogramDecoder : public ORVBasicTreeDecoder
{
  public:
    ORKatrinV4FLTEnergyHistogramDecoder() {fDataRecord = NULL; fHistogramLength = -1;}
    virtual ~ORKatrinV4FLTEnergyHistogramDecoder() {}

    enum EKatrinV4FLTEnergyHistogramConsts {kBufHeadLen = 12,
                                 kNumFLTChannels = 22};
    size_t fHistogramLength;
    
    virtual bool SetDataRecord(UInt_t* dataRecord);
    
    virtual inline UInt_t ChannelOf(UInt_t* record)
      { return ( record[1] & 0x0000ff00 ) >> 8; }
      
    //Functions that return data from buffer header:
    virtual inline UInt_t  ReadoutSecOf(UInt_t* record)  { return record[2]; }
    virtual inline UInt_t  RefreshTimeOf(UInt_t* record) { return record[3]; }
    virtual inline UInt_t  FirstBinOf(UInt_t* record)    { return record[4]; }
    virtual inline UInt_t  LastBinOf(UInt_t* record)     { return record[5]; }
    virtual inline UInt_t  HistogramLengthOf(UInt_t* record)    { return record[6]; }
    virtual inline UInt_t  MaxHistogramLengthOf(UInt_t* record) { return record[7]; }
    virtual inline UInt_t  BinSizeOf(UInt_t* record)     { return record[8]; }
    virtual inline UInt_t  OffsetEMinOf(UInt_t* record)  { return record[9]; }
    virtual inline UInt_t  HistogramIDOf(UInt_t* record)  { return record[10]; }
    virtual inline UInt_t  HistogramInfoOf(UInt_t* record)  { return record[11]; }

    // Histogram Functions
    inline UInt_t* GetHistogramDataPointer();
    virtual size_t CopyHistogramData(UInt_t* histogram, size_t len);

    // for basic trees
    virtual size_t GetNPars() { return 11; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* /*record*/) { return 1; }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    virtual std::string GetDataObjectPath() { return "ORKatrinV4FLTModel:KatrinV4FLTHistogram"; }
    //!< KatrinV4FLTHistogram is the key in ORKatrinV4FLTModel.m in - (NSDictionary*) dataRecordDescription -tb- 2008-07-25

  protected:
    UInt_t* fDataRecord;

};

//inline functions: ************************************************************************
inline UInt_t* ORKatrinV4FLTEnergyHistogramDecoder::GetHistogramDataPointer()
{
  return (fDataRecord + kBufHeadLen);
}


#endif
