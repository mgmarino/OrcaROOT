// ORTDC3377tdcDecoder.hh

#ifndef _ORTDC3377tdcDecoder_hh_
#define _ORTDC3377tdcDecoder_hh_

#include "ORVBasicRDTreeDecoder.hh"

class ORTDC3377tdcDecoder : public ORVBasicRDTreeDecoder
{
  public:
    ORTDC3377tdcDecoder() {}
    virtual ~ORTDC3377tdcDecoder() {}

    virtual inline bool IsDoubleWordTimestamp(UInt_t* record)
      { return (record[1] & 0x02000000) >> 25; }
    virtual inline UInt_t CrateOf(UInt_t* record)
      { return (record[1] & 0x01e00000) >> 21; }
    virtual inline UInt_t CardOf(UInt_t* record)
      { return (record[1] & 0x001f0000) >> 16; }
    virtual inline size_t NDataWordsOf(UInt_t* record)
      { return (record[1] & 0x0000ffff); }
    virtual double ReferenceDateOf(UInt_t* record);
    virtual inline size_t HeadOffOf(UInt_t* record)
      { return IsDoubleWordTimestamp(record) ? 8 : 6; }
    virtual inline bool HasValidHeader(UInt_t* record)
      { return (((UShort_t*)(record))[HeadOffOf(record)] & 0x8000); }
    virtual inline bool IsDoubleWordFormat(UInt_t* record)
      { return (((UShort_t*)(record))[HeadOffOf(record)] & 0x4000); }
    virtual inline bool IsShortWordFormat(UInt_t* record)
      { return !IsDoubleWordFormat(record); }
    virtual inline bool RecordsOnLeadingAndTrailingEdges(UInt_t* record)
      { return (((UShort_t*)(record))[HeadOffOf(record)] & 0x0400); }
    virtual inline bool RecordsOnLeadingEdgeOnly(UInt_t* record)
      { return !RecordsOnLeadingAndTrailingEdges(record); }

    virtual inline size_t NTDCValuesOf(UInt_t* record)
      { return IsDoubleWordFormat(record) ? NDataWordsOf(record)/2 : NDataWordsOf(record); }

    virtual inline UInt_t IthChannelOf(UInt_t* record, size_t i) 
      { return (GetTDCLocPtr(record, i)[0] & 0x7c00) >> 10; }
    virtual UInt_t IthTDCValueOf(UInt_t* record, size_t i);
    virtual bool IthTDCValueIsTrailingEdge(UInt_t* record, size_t i)
      { return RecordsOnLeadingEdgeOnly(record) ? false : GetTDCLocPtr(record, i)[0] & 0x0200; }
    virtual inline bool IthTDCValueIsLeadingEdge(UInt_t* record, size_t i)
      { return !IthTDCValueIsTrailingEdge(record, i); }

    virtual std::string GetDataObjectPath() { return "ORTDC3377:tdc"; }

    // for basic trees
    virtual size_t GetNPars() { return 5; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* record) { return NTDCValuesOf(record); }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);

  protected:
    virtual inline UShort_t* GetTDCLocPtr(UInt_t* record, size_t i)
      { i = IsDoubleWordFormat(record) ? i*2 + 1: i + 1; return ((UShort_t*)(record)) + HeadOffOf(record) + i; }
};

#endif
