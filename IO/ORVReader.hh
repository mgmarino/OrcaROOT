// ORVReader.hh

#ifndef _ORVReader_hh_
#define _ORVReader_hh_

#include <fstream>
#include "TSocket.h"
#include "ORHeaderDecoder.hh"
#include "ORBasicDataDecoder.hh"

class ORVReader 
{
  public:
    ORVReader();
    virtual ~ORVReader() {}
 
    virtual size_t Read(char* buffer, size_t nBytesMax) = 0; // return the number of bytes actually read
    virtual size_t ReadPartialLineWithCR(char* buffer, size_t nBytesMax);
    virtual bool ReadRecord(UInt_t*& buffer, size_t& nLongsMax); // allow buffer to be reallocated; requires "*&"
    virtual bool OKToRead() = 0;
    virtual bool Open() { fStreamVersion = ORHeaderDecoder::kUnknownVersion; return OpenDataStream(); }
    virtual bool OpenDataStream() = 0;
    virtual void Close() = 0;
 
    inline bool MustSwap() { return fMustSwap; }

  protected:
    virtual size_t DeleteAndResizeBuffer(UInt_t*& buffer, size_t newNLongsMax); 
    virtual void DetermineFileTypeAndSetupSwap(char* buffer);
    virtual bool ReadFirstWord(UInt_t*& buffer, size_t& nLongsMax);
    virtual bool ReadRestOfHeader(UInt_t*& buffer, size_t& nLongsMax);
    virtual bool ReadRestOfLongRecord(UInt_t*& buffer, size_t& nLongsMax);

  protected:
    ORHeaderDecoder::EOrcaStreamVersion fStreamVersion;
    ORBasicDataDecoder fBasicDecoder;
    ORHeaderDecoder fHeaderDecoder;
    bool fMustSwap;
};

#endif
