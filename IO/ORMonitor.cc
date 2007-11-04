// ORMonitor.cc

#include "ORMonitor.hh"
#include "ORLogger.hh"


TSocket* ORMonitor::fLastSocketRead = 0;

ORMonitor::ORMonitor(const char* host, int port) 
{
  fSocket = new TSocket(host, port);
  TMonitor::Add(fSocket, kRead);
  fSleepTime = 0;
  fKeepAlive = false;
  fReconnectAttempts = 0;
}

ORMonitor::ORMonitor(TSocket* aSocket)
{
  fSocket = aSocket;
  TMonitor::Add(fSocket, kRead);
  fSleepTime = 0;
  fKeepAlive = false;
  fReconnectAttempts = 0;
}

ORMonitor::~ORMonitor()
{
  delete fSocket;
}

bool ORMonitor::ResetSocket()
{
  /* use this function to reset a socket  */
  /* returns true if the socket is reset. */
  RemoveAll();
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
      TSocket* newSocket = new TSocket(fSocket->GetInetAddress(),
        fSocket->GetPort());
      if(newSocket->IsValid()) {
        TMonitor::Add(newSocket, kRead);
        delete fSocket;
        fSocket = newSocket;
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

