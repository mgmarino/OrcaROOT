// ORSocketWriter.cc

#include "ORSocketWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORSocketWriter::ORSocketWriter(const char* host, int port) :
ORMonitor(host, port)
{
}

size_t ORSocketWriter::Write(const char* /*buffer*/, size_t /*nBytesToWrite*/)
{
  /*
  if(!OKToWrite()) {
    ORLog(kError) << "Connection not available to write." << endl;
    return 0;
  }
  int nBytesSent = 0;//SendRaw(buffer, nBytesToWrite);
  if(nBytesSent == -1) ORLog(kError) << "Error transferring." << endl;
  if(nBytesSent == -4) ORLog(kError) << "Connection blocked." << endl;
  if(nBytesSent == -5) ORLog(kError) << "Pipe broken or reset by peer." << endl;
  
  return (nBytesSent < 0) ? 0 : nBytesSent;*/
  return 0;
}

