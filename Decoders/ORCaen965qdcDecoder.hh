// ORCaen965qdcDecoder.hh
//xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
//^^^^ ^^^^ ^^^^ ^^----------------------- ID (from header)
//-----------------^^ ^^^^ ^^^^ ^^^^ ^^^^- length
//xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
//--------^-^^^--------------------------- Crate number
//-------------^-^^^^--------------------- Card number
//xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
//^^^^ ^---------------------------------- Geo
//------^^^------------------------------- data type 0x2=header, 0x0=valid data,  0x4=end of block, 0x6=invalid
//-------------^-^^^---------------------- channel (V965)
//------------------^--------------------- RG (V965)
//-------------^-^^----------------------- channel (V965A)
//-----------------^---------------------- RG (V965A)
//-----------------------^---------------- under flow
//------------------------^--------------- over flow
//-------------------------^^^^ ^^^^ ^^^^- qdc value
//.... may be followed by more qdc words

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
    enum ORCaen965qdcDecoderConsts{kHeaderLength = 2};

    virtual size_t NValuesOf(UInt_t* record);
    virtual inline size_t NChannelsInBlock()
      { return (fRecord[0] & 0x3FFFF) - 2; }
    virtual inline size_t GetCrateOf()
      { return ((fRecord[1] >> 21) & 0x0000000f);}
    virtual inline size_t GetCardOf()
      { return ((fRecord[1] >> 16) & 0x0000001f);}
    virtual inline UInt_t GetIDFromHeader()
      { return ((fRecord[0] >> 18) & 0x0000cfff);}
    virtual inline UInt_t IthChannelOf(UInt_t* record, size_t iRow)
		{ return (GetLocPtr(record, iRow)[0] & 0x001e0000) >> 17; }
	virtual inline UInt_t IthRangeBitOf(UInt_t* record, size_t iRow)
		{ return (GetLocPtr(record, iRow)[0] & 0x00010000) >> 16; }
    virtual inline UInt_t IthValueOf(UInt_t* record, size_t iRow)
		{ return GetLocPtr(record, iRow)[0] & 0x00000fff; }
    virtual inline UInt_t IthValueIsUnderThreshold(UInt_t* record, size_t iRow)
		{ return (GetLocPtr(record, iRow)[0] & 0x00002000) >> 13; }
    virtual inline UInt_t IthValueIsOverflow(UInt_t* record, size_t iRow)
		{ return (GetLocPtr(record, iRow)[0] & 0x00001000) >> 12; }
    virtual inline UInt_t IthValueIsValid(UInt_t* record, size_t iRow)
		{ return (((GetLocPtr(record, iRow)[0] & 0x07000000) >> 24) == 0x000); }

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
      { return ((record[iWord] & 0x07000000) >> 24) == 0x000; }
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
