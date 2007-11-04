// ORSocketReader.cc

#include "ORSocketReader.hh"
#include "ORLogger.hh"

using namespace std;

ORSocketReader::ORSocketReader(const char* host, int port) :
  ORMonitor(host, port)
{
  fEOF = false; 
}

ORSocketReader::ORSocketReader(TSocket* aSocket) :
  ORMonitor(aSocket)
{
  fEOF = false; 
}

size_t ORSocketReader::Read(char* buffer, size_t nBytes)
{
  if(!OKToRead()) return 0;  
  if(nBytes==0) return nBytes;
  ORLog(kDebug) << "Waiting for data from socket..." << endl;
  TSocket* sock = Select();
  int nBytesRead = sock->RecvRaw(buffer, nBytes);
  ORLog(kDebug) << "Received " << nBytesRead << " bytes from socket." << endl;
  /* Receiving 0 bytes can mean that the socket has closed. */
  if (nBytesRead <= 0) {
    /* temp solution here.  This should really check to see if the connection
     * is still valid.  Unfortunately, there is a bug in root that was just
     * fixed. */
    if(!fKeepAlive) fEOF = true; 
    SetLastSocketRead(NULL);
  }
  else SetLastSocketRead(sock);
  return nBytesRead < 0 ? 0 : nBytesRead;
}

void ORSocketReader::Close()
{
  fEOF = !ResetSocket(); 
}
