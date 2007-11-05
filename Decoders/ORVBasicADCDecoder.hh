// ORVBasicADCDecoder.hh

#ifndef _ORVBasicADCDecoder_hh_
#define _ORVBasicADCDecoder_hh_

#include "ORVBasicRDTreeDecoder.hh"
#include "ORVHistDecoder.hh"
#include "ORVEventCounterDecoder.hh"

class ORVBasicADCDecoder : public ORVHistDecoder, public ORVBasicRDTreeDecoder, public ORVEventCounterDecoder
{
  public:
    ORVBasicADCDecoder() {}
    virtual ~ORVBasicADCDecoder() {}

    virtual void Swap(UInt_t* dataRecord);
    /* Handling the correct swapping for this record. */
    virtual inline UInt_t CrateOf(UInt_t* record)
      { return IsShort(record) ? (record[0] & 0x01e00000) >> 21 : (record[1] & 0x01e00000) >> 21; }
    virtual inline UInt_t CardOf(UInt_t* record)
      { return IsShort(record) ? (record[0] & 0x001f0000) >> 16 : (record[1] & 0x001f0000) >> 16; }
    virtual inline UInt_t ChannelOf(UInt_t* record)
      { return IsShort(record) ? (record[0] & 0x0000f000) >> 12 : (record[1] & 0x0000f000) >> 12; }
    virtual inline UInt_t ADCValueOf(UInt_t* record)
      { return IsShort(record) ? (record[0] & 0x00000fff) : (record[1] & 0x00000fff); }
    virtual double ReferenceDateOf(UInt_t* record);

    // for histograms
    virtual std::string GetHistName(int iHist);
    virtual std::string GetHistTitle(int iHist);
    virtual inline std::string GetXTitle() { return "ADC Value"; }
    virtual inline size_t GetNbinsX() { return 0xfff+1; }
    virtual inline double GetXLo() { return -0.5; }
    virtual inline double GetXHi() { return double(0xfff) + 0.5; }

    virtual int GetHistIndex(UInt_t* record);
    virtual inline double GetX(UInt_t* record, size_t /*i*/) { return double(ADCValueOf(record)); }

    // for basic trees
    virtual inline size_t GetNPars() { return 4; }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    // for event counters
    virtual inline size_t GetEventCount(UInt_t* /*record*/) { return 1; }

    virtual std::string GetLabel() = 0;
    virtual inline size_t GetNChannelsMax() { return 0xf+1; }
};

#endif
