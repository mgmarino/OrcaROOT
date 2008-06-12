// ORAD3511ADCDecoder.hh

#ifndef _ORAD3511ADCDecoder_hh_
#define _ORAD3511ADCDecoder_hh_

#include "ORVBasicRDTreeDecoder.hh"
#include "ORVHistDecoder.hh"

class ORAD3511ADCDecoder : public ORVHistDecoder, public ORVBasicRDTreeDecoder
{
  public:
    enum EAD3511ADCConsts { kNCards = 32 };

    ORAD3511ADCDecoder() {}
    virtual ~ORAD3511ADCDecoder() {}

    virtual void Swap(UInt_t* dataRecord);
    /* Handling the correct swapping for this record. */
    virtual inline bool HasReferenceDate(UInt_t* record)
      { return bool(record[1] & 0x02000000); }
    virtual inline size_t GetNADCValues(UInt_t* record)
      { return HasReferenceDate(record) ? LengthOf(record) - 4 : LengthOf(record) - 2; }
    virtual inline UInt_t IthADCValueOf(UInt_t* record, size_t i)
      { return HasReferenceDate(record) ? record[4+i] : record[2+i]; }
    virtual double ReferenceDateOf(UInt_t* record);

    // for histograms
    virtual std::string GetHistName(int iHist);
    virtual std::string GetHistTitle(int iHist);
    virtual std::string GetXTitle() { return "ADC Value"; }
    virtual size_t GetNbinsX() { return 0xfff+1; }
    virtual double GetXLo() { return -0.5; }
    virtual double GetXHi() { return double(0xfff) + 0.5; }

    virtual int GetHistIndex(UInt_t* record);
    virtual double GetX(UInt_t* record, size_t i) { return double(IthADCValueOf(record, i)); }

    // for basic trees
    virtual size_t GetNPars() { return 3; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* record) { return GetNADCValues(record); }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);

    virtual std::string GetDataObjectPath() { return "ORAD3511Model:ADC"; }
};

#endif
