// ORSocketReader.hh

#ifndef _ORSocketReader_hh_
#define _ORSocketReader_hh_

#include "ORMonitor.hh"
#include "ORVReader.hh"

class ORSocketReader : public ORMonitor, public ORVReader
{
  public:
    ORSocketReader(const char* host, int port);
    ORSocketReader(TSocket* aSocket);
    virtual ~ORSocketReader() {}

    virtual size_t Read(char* buffer, size_t nBytes);
    virtual bool OKToRead() { return GetActive() > 0 && !fEOF; }
    //virtual bool OKToRead() { return IsValid() && !fEOF; }
    //virtual bool OKToRead() { return IsValid() && !fEOF && Status(kRead, 1000); }
    virtual bool OpenDataStream() {return OKToRead();} 
    virtual void Close(); // do nothing: stay connected and wait for next run to start

  protected:
    
    bool fEOF;
};

#endif
