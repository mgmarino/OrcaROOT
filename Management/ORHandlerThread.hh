// ORHandlerThread.hh

#ifndef _ORHandlerThread_hh
#define _ORHandlerThread_hh

/* This class handles all the signals and signal passing to
   ORVSignaHandlers.  It should be called from the main thread before
   any other threads are started. */

#include <pthread.h>
#include "ORReadWriteLock.hh"
class ORHandlerThread 
{
  public:
    ORHandlerThread();
    ~ORHandlerThread();

    void StartThread();
    void StopThread();

  protected:
    static void* SigWaitThread(void*);
    bool fThreadIsRunning;
    bool fCanIStart;
    pthread_t fThread; 
    pthread_attr_t fAttr;
    static ORReadWriteLock fRWLock;
    static ORHandlerThread* fHandlerThread;
    static sigset_t fSigsToBlock;

};
#endif
