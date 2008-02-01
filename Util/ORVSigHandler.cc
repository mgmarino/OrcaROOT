// ORVSigHandler.cc

#include "ORVSigHandler.hh"
#include <signal.h>
#include <algorithm>
#include "ORLogger.hh"

using namespace std;

vector<ORVSigHandler*> ORVSigHandler::fgHandlers;
ORReadWriteLock ORVSigHandler::fgBaseRWLock;

ORVSigHandler::ORVSigHandler()
{
  fgBaseRWLock.writeLock();
  fgHandlers.push_back(this);
  fgBaseRWLock.unlock();
  fSetToCancel = false;
}

ORVSigHandler::~ORVSigHandler()
{ 
  fgBaseRWLock.writeLock();
  vector<ORVSigHandler*>::iterator it = find(fgHandlers.begin(), fgHandlers.end(), this);
  if (it != fgHandlers.end()) {
    fgHandlers.erase(it);
  } else {
    ORLog(kWarning) << "couldn't find this-ptr in fgHandlers! "
                    << "Something is very wrong..." << endl;
  }
  fgBaseRWLock.unlock();
}

void ORVSigHandler::CancelAll() 
{
  fgBaseRWLock.readLock();
  for (size_t i=0;i<fgHandlers.size();i++) {
    fgHandlers[i]->CancelAnInstance(); 
  } 
  fgBaseRWLock.unlock();
}

void ORVSigHandler::CancelAnInstance()
{
  fRWLock.writeLock();
  fSetToCancel = true;
  fRWLock.unlock();
}

bool ORVSigHandler::TestCancel() 
{
  bool testCancel;  
  fRWLock.readLock();
  testCancel = fSetToCancel;
  fRWLock.unlock();
  return testCancel;
}

