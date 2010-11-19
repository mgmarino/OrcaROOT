// ORCaen965qdcDecoder.hh

#ifndef _ORCaen965qdcDecoder_hh_
#define _ORCaen965qdcDecoder_hh_

#include <vector>
#include "ORVBasicTreeDecoder.hh"

#include <iostream>

class ORCaen965qdcDecoder : public ORVBasicTreeDecoder
{
  public:
    ORCaen965qdcDecoder();
    virtual ~ORCaen965qdcDecoder() {}

    virtual size_t NValuesOf(UInt_t* record);
    virtual inline UInt_t IthChannelOf(UInt_t* record, size_t iRow)
		{ return (GetLocPtr(record, iRow)[0] & 0x001E0000) >> 17; }
	virtual inline UInt_t IthRangeBitOf(UInt_t* record, size_t iRow)
		{ return (GetLocPtr(record, iRow)[0] & 0x00010000) >> 16; }
    virtual inline UInt_t IthValueOf(UInt_t* record, size_t iRow)
		{ return GetLocPtr(record, iRow)[0] & 0x00000fff; }
    virtual inline UInt_t IthValueIsUnderThreshold(UInt_t* record, size_t iRow)
		{ return (GetLocPtr(record, iRow)[0] & 0x00002000) >> 13; }
    virtual inline UInt_t IthValueIsOverflow(UInt_t* record, size_t iRow)
		{ return (GetLocPtr(record, iRow)[0] & 0x00001000) >> 12; }
    virtual inline UInt_t IthValueIsValid(UInt_t* record, size_t iRow)
		{ return ((GetLocPtr(record, iRow)[0] & 0x07000000) >> 24) == 0x000; }

    virtual std::string GetDataObjectPath() { return "ORCaen965Model:Qdc"; }
    virtual std::string GetValueName()		{ return "ORCaen965DecoderForQdc"; }

    // for basic trees
    virtual size_t GetNPars() { return 7; }
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* record) { return NValuesOf(record); }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow);

  protected:
    virtual void LoadLocPtrs(UInt_t* record);
    virtual inline UInt_t IthWordIsData(UInt_t* record, size_t iWord)
      { return ((record[iWord] & 0x07000000) >> 24) == 0x0; }
    virtual inline size_t NChannelsInBlock(UInt_t* headerPtr)
      { return (headerPtr[0] & 0x3FFFF) - 2; }
    virtual UInt_t* GetLocPtr(UInt_t* record, size_t i);
    std::vector<UInt_t*> fLocPtrs;
    UInt_t* fRecord;
};

class ORCaen965AqdcDecoder : public ORCaen965qdcDecoder
{
	public:
		virtual inline UInt_t IthChannelOf(UInt_t* record, size_t iRow)
			{ return (GetLocPtr(record, iRow)[0] & 0x001C0000) >> 18; }
	virtual inline UInt_t IthRangeBitOf(UInt_t* record, size_t iRow)
			{ return (GetLocPtr(record, iRow)[0] & 0x00020000) >> 17; }

	virtual std::string GetDataObjectPath() { return "ORCaen965Model:965AQdc"; }
    virtual std::string GetValueName()		{ return "ORCaen965ADecoderForQdc"; }
};

#endif
