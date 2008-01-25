// ORMonitor.cc

#include "ORMonitor.hh"
#include "ORLogger.hh"


TSocket* ORMonitor::fSocketToWrite = 0;

ORMonitor::ORMonitor(const char* host, int port, bool writable) 
{
  TSocket* aSocket = new TSocket(host, port);
  TMonitor::Add(aSocket, kRead);
  fSleepTime = 0;
  fKeepAlive = false;
  fReconnectAttempts = 0;
  fIsWritable = writable;
  /* If the socket is writable, then we can't add anymore sockets to the monitor.*/
  if (fIsWritable) fSocketToWrite = aSocket;
}

ORMonitor::ORMonitor(TSocket* aSocket, bool writable)
{
  TMonitor::Add(aSocket, kRead);
  fSleepTime = 0;
  fKeepAlive = false;
  fReconnectAttempts = 0;
  fIsWritable = writable;
  /* If the socket is writable, then we can't add anymore sockets to the monitor.*/
  if (fIsWritable) fSocketToWrite = aSocket;
}

bool ORMonitor::ResetSocket(TSocket* sock)
{
  /* use this function to reset a socket  */
  /* returns true if the socket is reset. */
  if (!sock) return false;
  Remove(sock);
  sock->Close();
  if(fKeepAlive) {
    static unsigned int i = 0;
    static bool keepCycling = (fReconnectAttempts==0) ? true : (i < fReconnectAttempts); 
    while(keepCycling) {
      ORLog(kRoutine) << "Waiting " << fSleepTime << " seconds to reset connection" 
        << std::endl;
      if(fReconnectAttempts > 0) {
        ORLog(kDebug) << "Attempt: " << (i+1) << " of " << fReconnectAttempts << std::endl;
      }
      sleep(fSleepTime); 
      TSocket* newSocket = new TSocket(sock->GetInetAddress(),
        sock->GetPort());
      if(newSocket->IsValid()) {
        TMonitor::Add(newSocket, kRead);
        return true;
      } else {
        delete newSocket;
      }
      i++;
      keepCycling = (fReconnectAttempts==0) ? true : (i < fReconnectAttempts); 
    }
  }
  return false;
}

void ORMonitor::AddSocket(TSocket* sock, Int_t /*interest*/)
{
  /* We only allow one socket if there is a writable socket. */
  /* Also, we only allow read sockets to be read. */
  if (!fIsWritable) return TMonitor::Add(sock, kRead);
  ORLog(kWarning) << "Only one socket is allowed when there is a writable socket" << std::endl;
}
 

