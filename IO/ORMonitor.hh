// ORSocket.hh

#ifndef _ORMonitor_hh_
#define _ORMonitor_hh_

#include "TMonitor.h"
#include "TSocket.h"

class ORMonitor : public TMonitor
{
  public:
    ORMonitor(const char* host, int port);
    ORMonitor(TSocket* aSocket);
    virtual ~ORMonitor(); 

    virtual bool ResetSocket();
    /* Resets a socket by first closing and re-opening.  Also removes from
     * list os active sockets.  */

    static TSocket* GetLastSocketRead() {return fLastSocketRead;} 
    /* Returns the socket which was read last.  This is useful if there are a
     * number of sockets being read and one wants to write to the last one
     * which read in. */
    virtual void SetSleepTime(unsigned long numSecs) {fSleepTime = numSecs;}
    virtual void SetKeepAlive(bool keepAlive) {fKeepAlive = keepAlive;}
    virtual void SetReconnectAttempts(bool attempts) {fReconnectAttempts = attempts;}
    virtual void Add(TSocket*, Int_t) {} 
    /* Overloading, one shouldn't be adding sockets to ORMonitor*/

  protected:
    static TSocket* fLastSocketRead;
    TSocket* fSocket;
    void SetLastSocketRead(TSocket* sock) {fLastSocketRead = sock;}
    unsigned long fSleepTime;
    unsigned int fReconnectAttempts;
    bool fKeepAlive;
};

#endif
