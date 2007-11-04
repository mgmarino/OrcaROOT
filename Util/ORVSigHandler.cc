// ORVSigHandler.cc

#include "ORVSigHandler.hh"
#include <signal.h>
#include <algorithm>
#include "ORLogger.hh"

using namespace std;

vector<ORVSigHandler*> ORVSigHandler::fgHandlers;
map<int, bool> ORVSigHandler::fgHaveHandled;

ORVSigHandler::ORVSigHandler()
{
  (void) signal(SIGINT, &ORVSigHandler::HandleAll);
  fgHandlers.push_back(this);
}

ORVSigHandler::~ORVSigHandler()
{ 
  vector<ORVSigHandler*>::iterator it = find(fgHandlers.begin(), fgHandlers.end(), this);
  if (it != fgHandlers.end()) {
    fgHandlers.erase(it);
  } else {
    ORLog(kWarning) << "couldn't find this-ptr in fgHandlers! "
                    << "Something is very wrong..." << endl;
  }
}

void ORVSigHandler::HandleAll(int signal)
{ 
  // The check using fgHaveHandled is my poor-man's multithreaded signal
  // processing hack. See details in ORProcessStopper.
  for(size_t i=0; i<fgHandlers.size(); i++) {
    if(!fgHaveHandled[i]) {
      fgHaveHandled[i] = true; // set true first so process isn't killed twice...
      fgHandlers[i]->Handle(signal);
    }
  }
  exit(signal);
}


