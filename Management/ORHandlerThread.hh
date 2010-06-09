// ORHandlerThread.hh

#ifndef _ORHandlerThread_hh
#define _ORHandlerThread_hh

/* This class handles all the signals and signal passing to
   ORVSignaHandlers.  It MUST BE called from the main thread before
   any other threads are started. This is because it sets to block
   SIGINT, and every derived thread inherits this signal block.  
   It is easiest just to call this before any other OR object. 

   The proper call to make is like:
   ORHandlerThread handler;
   handler.StartThread()

   StopThread() does not need to be called, nor does it really work
   as advertised due to some inconsistencies between Mac os x, posix,
   linux standards etc.  Therefore, if you don't want a signal handling
   thread, then don't instantiate this class. 
   */

#include <pthread.h>
#include <signal.h>
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
