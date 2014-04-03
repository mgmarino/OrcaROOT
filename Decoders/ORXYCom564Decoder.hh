// ORXYCom564Decoder.hh

#ifndef _ORXYCom564Decoder_hh_
#define _ORXYCom564Decoder_hh_

#include "ORVBasicADCDecoder.hh"

class ORXYCom564Decoder : public ORVBasicADCDecoder
{
  protected: 
    enum { kXYCom564HeaderSize = 4, 
           kXYCom564MaxChannels = 64 };
  public:

    // Disable because we have more than one ADC value in a record 
    virtual inline UInt_t ADCValueOf(UInt_t* /*record*/) { return 0; }

    virtual double ReferenceDateOf(UInt_t* record);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);

    virtual inline std::string GetLabel() { return "XYCom564"; }
    virtual inline std::string GetDataObjectPath() { return "ORXYCom564Model:XYCom564"; }
    virtual inline std::string GetDictionaryObjectPath() { return "ORXYCom564Model"; }
    

    virtual inline double GetXHi() { return double(0xffff) + 0.5; }
    virtual inline size_t GetNChannelsMax() { return kXYCom564MaxChannels; }
    virtual size_t GetNRows(UInt_t* record) 
      { return (LengthOf(record) < kXYCom564HeaderSize) ? 
          0 : (LengthOf(record) - kXYCom564HeaderSize); }


};

#endif /* _ORXYCom564Decoder_hh_ */
