// ORVSigHandler.cc

#include "ORVSigHandler.hh"
#include <signal.h>
#include <algorithm>
#include "ORLogger.hh"

using namespace std;

map<pthread_t, vector<ORVSigHandler*> > ORVSigHandler::fgHandlers;
ORReadWriteLock ORVSigHandler::fgBaseRWLock;

ORVSigHandler::ORVSigHandler()
{
  fgBaseRWLock.writeLock();
  fMyThread = pthread_self();
  fgHandlers[fMyThread].push_back(this);
  fgBaseRWLock.unlock();
  fSetToCancel = false;
}

ORVSigHandler::~ORVSigHandler()
{ 
  fgBaseRWLock.writeLock();
  std::map<pthread_t, std::vector<ORVSigHandler*> >::iterator mapIt = 
    fgHandlers.find(fMyThread);
  if (mapIt == fgHandlers.end()) {
    ORLog(kWarning) << "couldn't find this-ptr in fgHandlers! "
                    << "Something is very wrong..." << endl;
    fgBaseRWLock.unlock();
    return;
  }
  vector<ORVSigHandler*>::iterator it = 
    find(mapIt->second.begin(), mapIt->second.end(), this);
  if (it != mapIt->second.end()) {
    mapIt->second.erase(it);
  } else {
    ORLog(kWarning) << "couldn't find this-ptr in fgHandlers! "
                    << "Something is very wrong..." << endl;
  }
  fgBaseRWLock.unlock();
}

void ORVSigHandler::CancelAllInThread(pthread_t aThread) 
{
  fgBaseRWLock.readLock();
  std::map<pthread_t, std::vector<ORVSigHandler*> >::iterator mapIt = 
    fgHandlers.find(aThread);
  if (mapIt == fgHandlers.end()) return; 
  for (size_t i=0;i<mapIt->second.size();i++) {
    mapIt->second[i]->CancelAnInstance(); 
  }
  fgBaseRWLock.unlock();
}


void ORVSigHandler::CancelAll() 
{
  fgBaseRWLock.readLock();
  std::map<pthread_t, std::vector<ORVSigHandler*> >::iterator mapIt; 
  for (mapIt=fgHandlers.begin();mapIt!=fgHandlers.end();mapIt++) {
    for (size_t i=0;i<mapIt->second.size();i++) {
      mapIt->second[i]->CancelAnInstance(); 
    }
  } 
  fgBaseRWLock.unlock();
}

void ORVSigHandler::CancelAnInstance()
{
  fRWLock.writeLock();
  fSetToCancel = true;
  fRWLock.unlock();
}

void ORVSigHandler::UnCancel()
{
  fRWLock.writeLock();
  fSetToCancel = false;
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

