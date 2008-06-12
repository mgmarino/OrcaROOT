// ORJAMFADCDecoder.hh

#ifndef _ORJAMFADCDecoder_hh_
#define _ORJAMFADCDecoder_hh_

#include "ORVBasicRDTreeDecoder.hh"
#include "ORVHistDecoder.hh"

class ORJAMFADCDecoder : public ORVHistDecoder, public ORVBasicRDTreeDecoder
{
  public:
    enum EJAMFADCConsts { kNCards = 32 };

    ORJAMFADCDecoder() {}
    virtual ~ORJAMFADCDecoder() {}

    virtual void Swap(UInt_t* dataRecord);

    virtual double ReferenceDateOf(UInt_t* record)
      { return (double) record[2]; }
    virtual inline size_t GetNChannels(UInt_t* record)
      { return LengthOf(record) - 3; }
    virtual inline UInt_t IthChannelOf(UInt_t* record, size_t iChannel)
      { return (record[3+iChannel] & 0xf0000) >> 16; }
    virtual inline UInt_t IthADCValueOf(UInt_t* record, size_t iChannel)
      { return record[3+iChannel] & 0xffff; }

    // for histograms
    virtual std::string GetHistName(int iHist);
    virtual std::string GetHistTitle(int iHist);
    virtual std::string GetXTitle() { return "ADC Value"; }
    virtual std::string GetYTitle() { return "Channel"; }

    virtual inline size_t GetNDim() { return 2; }
    virtual size_t GetNbinsX() { return 0xffff+1; }
    virtual size_t GetNbinsY() { return 0xf+1; }

    virtual inline int GetHistIndex(UInt_t* record) 
      { return CardOf(record) + kNCards*CrateOf(record); }
    virtual inline size_t GetNEntries(UInt_t* record)
      { return GetNChannels(record); }

    virtual inline double GetX(UInt_t* record, size_t i) 
      { return double(IthADCValueOf(record, i)); }
    virtual inline double GetY(UInt_t* record, size_t i) 
      { return double(IthChannelOf(record, i)); }

    // for basic trees
    virtual size_t GetNPars() { return 4; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* record) { return GetNChannels(record); }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);

    virtual std::string GetDataObjectPath() { return "JAMF:JAMFADC"; }
};

#endif
