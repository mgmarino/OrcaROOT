// ORSocketWriter.hh
/* Defines a class to handle socket writing.  The interface is modelled after
 * the ORVReader interface but it doesn't seem necessary to write a virtual
 * interface to handle writers.
 */
#ifndef _ORSocketWriter_hh_
#define _ORSocketWriter_hh_

#include "ORMonitor.hh"

class ORSocketWriter : public ORMonitor 
{
  
  public:
    ORSocketWriter(const char* host, int port);
    virtual ~ORSocketWriter() {}

    virtual bool OKToWrite() { return GetActive()!=0; }
    virtual bool OpenDataStream() { return OKToWrite(); } // do nothing: should already be open from constructor
    virtual size_t Write(const char* buffer, size_t nBytesToWrite);

};

#endif
