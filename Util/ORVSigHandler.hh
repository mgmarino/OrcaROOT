// ORVSigHandler.hh

#ifndef _ORVSigHandler_hh_
#define _ORVSigHandler_hh_

#include <vector>
#include <map>
#ifndef __CINT__
#include <pthread.h>
#else
// Dealing with CINT
typedef struct { private: char x[SIZEOF_PTHREAD_T]; } pthread_t;
#endif
#ifndef _ORReadWriteLock_hh
#include "ORReadWriteLock.hh"
#endif

//! Allows asynchronous signals to be handles synchronously 
/*! 
   This class can be called asynchronously (e.g. from a signal handler, or another thread)
   and we use it to signal derived classes synchronously.  

   The idea is simple: a class that derives from this can periodically
   call TestCancel() to determine if it needs to exit, or do something.  
   This is particularly useful in the case of a reader, which needs to 
   close, or a processor which is looping and might be requested to close
   in the middle of a loop.  Nothing here guarantees that a process will
   stop.  Instead this sets markers (like pthread_testcancel, except TestCancel
   does not immediately exit the thread if there is a pending cancel) and a class
   can test these at appropriate times. 

   This class is safe across thread boundaries, but with some caveats. If CancelAll()
   is called, then TestCancel() will be set to true for *all* derived classes in every
   thread.  If one wants to signal only a particular thread then call CancelAllInThread()
   which will set TestCancel() to true for all derived classes within the specified
   thread.  (There is no error if the thread doesn't exist, or if no derived classes 
   exist in that thread.)  Derived classes take the pthread_t id of the thread in which
   they were generated, so whereas all calls to CancelAll and CancelAllInThread are thread-safe
   (from any thread) the programmer needs to know exactly which classes are being told to 
   cancel.   

   Also, this class does not provide signal handling, or any heavy-handed kill functionality.
   It WILL NOT kill, delete, or stop a derived class.  It is the coder's responsibility to set
   test TestCancel() at appropriate locations like:

   \verbatim
   if (TestCancel()) {
     // I need to exit.  Do something, clean up, and get out.
   }
   // otherwise continue as normal
   \endverbatim

  For signal handling, please see ORHandlerThread.  

  It is important for the coder to be aware of blocking system calls that might not exit.  Some
  workarounds can be done to keep this from happening.
*/

class ORVSigHandler
{
  public:
    //! Call to cancel all ORVSigHandlers.
    static void CancelAll();

    //! Just call cancel all in a particular thread. 
    static void CancelAllInThread(pthread_t aThread);
    
    //! Cancels just the particular class.
    void Cancel() { CancelAnInstance(); }

    //! Reset the Cancel flags.
    void UnCancel();
    
  protected:
    ORVSigHandler();
    virtual ~ORVSigHandler();

    //! This function indicates to the class whether or not it has been canceled.
    /*!
     * In particular one should use this in a loop that should be stopped by
     * an interrupt.  It is the coders responsibility to make sure these are
     * placed appropriately.  See, e.g., ORDataProcManager for how to do this. 
     */
    bool TestCancel();
    void CancelAnInstance();


  private:
    ORReadWriteLock fRWLock;
    bool fSetToCancel;
    static ORReadWriteLock fgBaseRWLock;
    static std::map<pthread_t, std::vector<ORVSigHandler*> > fgHandlers;
    pthread_t fMyThread;
    
};
#endif /* _ORVSigHandler_hh_ */
