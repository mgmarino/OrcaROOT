// ORCaenV830Decoder.hh

#ifndef _ORCaenV830Decoder_hh_
#define _ORCaenV830Decoder_hh_

#include "ORVBasicTreeDecoder.hh"
#include "ORVEventCounterDecoder.hh"
#include <cstdio>

class ORCaenV830Decoder : public virtual ORVBasicTreeDecoder, public virtual ORVEventCounterDecoder
{
  public:
     
    ORCaenV830Decoder() {}
    virtual ~ORCaenV830Decoder() {}

    // You always should treat the counter and overflow values as unsigned. A 'bad read' will just be 0xffffffff 
    // in BOTH the overflow and counter values. You should never have to treat them as signed values. 
   
    virtual inline size_t NScalersOf(UInt_t* record) // sets the total number of channels (should be 1 for MJD)
      { return LengthOf(record) - 5; }  	
    virtual UInt_t IthChannelOf(UInt_t* record, size_t iRow);
    virtual inline UInt_t CallAsInt(UInt_t* record, int i)
      { return record[i]; }

    virtual inline UInt_t GetEnabledMask(UInt_t* record)
      { return record[3]; }
    virtual inline UInt_t GetChan0RollOver(UInt_t* record)
      { return record[2]; }
    virtual inline UInt_t GetHeader(UInt_t* record)
      { return record[4]; }
    virtual inline UInt_t GetCounter(UInt_t* record, size_t iRow)
      { return record[5 + iRow]; }
    virtual inline ULong_t GetCounter0Long(UInt_t* record)
      { return ((ULong_t)GetChan0RollOver(record) << 32) | GetCounter(record, 0); }
    virtual inline size_t GetEventCount(UInt_t* record)
      { return GetHeader(record) & 0xFFFF; }

  // Event branch
    virtual inline std::string GetLabel() { return "ORCV830DecoderForEvent"; }
    virtual inline std::string GetDataObjectPath() 
      { return "ORCV830Model:Event"; }
    
    // Polled Read branch (not needed by MJD as of 2/15/15)
    //virtual inline std::string GetLabel() { return "ORCV830DecoderForPolledRead"; }
    //virtual inline std::string GetDataObjectPath() 
    //  { return "ORCV830Model:PolledRead"; }

    // for basic trees
    virtual size_t GetNPars() { return 7; }	      // # of variables of interest 
    virtual std::string GetParName(size_t iPar);
    virtual size_t GetNRows(UInt_t* record) { return NScalersOf(record); }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow); 
};

#endif
