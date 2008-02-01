// ORVSigHandler.hh

#ifndef _ORVSigHandler_hh_
#define _ORVSigHandler_hh_

#include <vector>
#include "ORReadWriteLock.hh"
/* This class is called asynchronously, so we use it to try
   and take things down in-sync.  That is, we wait for a
   safe place and then shut down. */

class ORVSigHandler
{
  public:
    static void CancelAll();
    
  protected:
    ORVSigHandler();
    virtual ~ORVSigHandler();

    bool TestCancel();
    /* This function indicates to the class whether or not it has been canceled.*/
    /* In particular one should use this in a loop that should be stopped by
       an interrupt.  It is the coders responsibility to make sure these are
       placed appropriately.  See, e.g., ORDataProcManager for how to do this. */
    void CancelAnInstance();

    static std::vector<ORVSigHandler*> fgHandlers;

  private:
    ORReadWriteLock fRWLock;
    bool fSetToCancel;
    static ORReadWriteLock fgBaseRWLock;
    
};

#endif
