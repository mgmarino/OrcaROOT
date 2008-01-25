// ORSocket.hh

#ifndef _ORMonitor_hh_
#define _ORMonitor_hh_

#include "TMonitor.h"
#include "TSocket.h"

/* Limit the access into TMonitor. */
class ORMonitor : protected TMonitor
{
  public:
    ORMonitor(const char* host, int port, bool writable = false);
    ORMonitor(TSocket* aSocket, bool writable = false);
    /* Setup a monitor with a socket.  If one wants to allow the socket to
       be writable then set writable to true.  This is useful in the case of
       orcaroot being run as a daemon.  Currently, we're limiting the amount
       of sockets to one if there is a writable socket.  */

    virtual ~ORMonitor() {}


    static TSocket* GetSocketToWrite() {return fSocketToWrite;} 
    /* Returns the socket which was read last.  This is useful if there are a
     * number of sockets being read and one wants to write to the last one
     * which read in. */
    virtual void SetSleepTime(unsigned long numSecs) {fSleepTime = numSecs;}
    virtual void SetKeepAlive(bool keepAlive) {fKeepAlive = keepAlive;}
    virtual void SetReconnectAttempts(bool attempts) {fReconnectAttempts = attempts;}
    virtual void AddSocket(TSocket* sock, Int_t interest);

    /* since we inherited from TMonitor as protected, and root overloaded(!) 
       new and delete, we need to put them back in. */
    void* operator new (size_t sz) {return TMonitor::operator new(sz);}
    void* operator new[] (size_t sz) {return TMonitor::operator new[](sz);}
    void* operator new (size_t sz, void* vp) {return TMonitor::operator new(sz, vp);}
    void* operator new[] (size_t sz, void* vp) {return TMonitor::operator new[](sz, vp);}
    void operator delete (void* ptr) {TMonitor::operator delete (ptr);}
    void operator delete[] (void* ptr) {TMonitor::operator delete[] (ptr);}
    void operator delete (void* ptr, void* vp) {TMonitor::operator delete[] (ptr,vp);}
    void operator delete[] (void* ptr, void* vp) {TMonitor::operator delete[] (ptr,vp);}

  protected:

    ORMonitor() {} // this should never be called.
    virtual bool ResetSocket(TSocket* sock);
    /* Resets a socket by first closing and re-opening.  Also removes from
     * list of active sockets.  */
    /* Make sure this socket was in the active list, i.e. received from Select()*/
    static TSocket* fSocketToWrite;
    unsigned long fSleepTime;
    unsigned int fReconnectAttempts;
    bool fKeepAlive;
    bool fIsWritable;
    
};

#endif
