#include "ORHandlerThread.hh"
#include "ORVSigHandler.hh"
#include "ORLogger.hh"
#include <signal.h>
#include <cstdlib>

ORReadWriteLock ORHandlerThread::fRWLock;
ORHandlerThread* ORHandlerThread::fHandlerThread = 0;
sigset_t ORHandlerThread::fSigsToBlock;

ORHandlerThread::ORHandlerThread()
{
  fRWLock.writeLock();
  if (fHandlerThread != 0) {
    ORLog(kError) << "ORHandlerThread should only be made once!" << std::endl; 
    fCanIStart = false;
    fThreadIsRunning = false;
    fRWLock.unlock(); 
    return;
  } 
  fHandlerThread = this;
  fRWLock.unlock();
  fCanIStart = true;
  fThreadIsRunning = false;

  sigemptyset(&fSigsToBlock);
  sigaddset(&fSigsToBlock, SIGINT); //We only are considering the interrupt.
  if( pthread_sigmask(SIG_BLOCK, &fSigsToBlock, NULL) != 0) {
    ORLog(kError) << "Error setting up block signals." << std::endl; 
    fCanIStart = false;
  }
}

ORHandlerThread::~ORHandlerThread()
{
  if (fThreadIsRunning) StopThread();
  fRWLock.writeLock();
  fHandlerThread = NULL;
  fRWLock.unlock();
}

void ORHandlerThread::StartThread()
{
  if (fThreadIsRunning || !fCanIStart) return; 
  pthread_attr_init(&fAttr);                                                       
  pthread_attr_setdetachstate(&fAttr, PTHREAD_CREATE_JOINABLE);
  int retCode = pthread_create(&fThread, &fAttr, SigWaitThread, NULL); 
  if (retCode) {
    ORLog(kError) << "Error starting SigWaitThread" << std::endl;
    return;
  }  
  fThreadIsRunning = true;
  pthread_attr_destroy(&fAttr);                                                       
}

void ORHandlerThread::StopThread()
{
  if (!fThreadIsRunning) return;
  pthread_cancel(fThread);  
  /* Wait for it to stop. */
  //pthread_join(fThread, NULL);
}

void* ORHandlerThread::SigWaitThread(void*)
{
  pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
  int numTimesCalled = 0;
  int sig_caught;
  while (numTimesCalled < 2) {
    if (sigwait(&fSigsToBlock, &sig_caught) != 0) continue; 
    if (sig_caught == SIGINT) {
      numTimesCalled++;
      ORVSigHandler::CancelAll();
    }
  }
  exit(SIGINT);
}
