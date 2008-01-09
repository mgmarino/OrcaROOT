// ORTrig4ChanDecoder.hh

#ifndef __ORTrig4ChanDecoder_hh__
#define __ORTrig4ChanDecoder_hh__

#include "ORVBasicTreeDecoder.hh"

class ORTrig4ChanDecoder : public ORVBasicTreeDecoder
  // class ORTrig4ChanDecoder : public ORVHistDecoder, public ORVBasicTreeDecoder
{
  public:
    ORTrig4ChanDecoder() {}
    virtual ~ORTrig4ChanDecoder() {}

    virtual inline UInt_t trigOf(UInt_t* record)
      { return (record[1] & 0x07000000) >> 24; }
    virtual inline UInt_t UpClockOf(UInt_t* record)
      { return (record[1] & 0x00ffffff);}
    virtual inline UInt_t LowClockOf(UInt_t* record)
      { return (record[2] & 0xffffffff); }

    // for basic trees
    virtual inline size_t GetNPars() { return 3; }
    virtual std::string GetParName(size_t iPar);
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);

    virtual inline std::string GetDataObjectPath() { return "OR4ChanModel:100MHz Clock"; }
};

#endif
