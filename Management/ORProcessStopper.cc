// ORProcessStopper.cc

#include <algorithm>
#include <iostream>
#include <signal.h>
#include "ORLogger.hh"
#include "ORProcessStopper.hh"

using namespace std;

bool ORProcessStopper::fgStopNow = false;
pthread_mutex_t ORProcessStopper::fgStopMutex;
map<string, bool> ORProcessStopper::fgIsAQuitVariation;
map<string, bool> ORProcessStopper::fgIsAQuitNowVariation;
map<string, bool> ORProcessStopper::fgIsAHelpVariation;
const char ORProcessStopper::fgHelp[] = 
"\n"
"Available commands [long form (short form)]:\n"
"- help (h) : show this information\n"
"- quit/stop/exit (q) : stop processing after this run is over\n"
"- quit/stop/exit now (qn/qq) : stop after processing the current data record\n"
"\n";

ORProcessStopper::ORProcessStopper()
{
  pthread_mutex_init(&fgStopMutex, NULL);

  fgIsAQuitVariation["quit"] = true;
  fgIsAQuitVariation["stop"] = true;
  fgIsAQuitVariation["exit"] = true;
  fgIsAQuitVariation["q"] = true;

  fgIsAQuitNowVariation["quit now"] = true;
  fgIsAQuitNowVariation["stop now"] = true;
  fgIsAQuitNowVariation["exit now"] = true;
  fgIsAQuitNowVariation["qn"] = true;
  fgIsAQuitNowVariation["qq"] = true;

  fgIsAHelpVariation["help"] = true;
  fgIsAHelpVariation["h"] = true;
}

ORProcessStopper::~ORProcessStopper()
{
  pthread_mutex_destroy(&fgStopMutex);
  // remove quit file here, just in case...
  if(access("quit", F_OK) == 0) {
    system("rm quit");
  }
}

int ORProcessStopper::ExecuteStopperThread() 
{
  pthread_attr_init(&fAttr);
  pthread_attr_setdetachstate(&fAttr, PTHREAD_CREATE_JOINABLE);

  int retCode = pthread_create(&fThread, &fAttr, StopperThread, NULL);
  if (retCode) {
    ORLog(kError) << "ExecuteStopperThread(): return code from pthread_create() is " 
                  << retCode << endl;
  }
  return retCode;
}

void* ORProcessStopper::StopperThread(void* /*arg*/)
{
  sleep(1); // allow some time for other messages to go by...
  string command = "";
  while (1) {
    cout << "enter command (e.g. help): ";
    cin >> command;
    for(unsigned i=0; i<command.size(); i++) {
      command[i] = tolower(command[i]);
    }
    if (fgIsAQuitNowVariation[command]) {
      Lock();
      fgStopNow = true;
      Unlock();
      system("touch quit"); // removed in Stop or deconstructor
      ORLog(kRoutine) << "Will stop after current data record." << endl;
      break;
    }
    if (fgIsAQuitVariation[command]) {
      system("touch quit"); // removed in Stop or deconstructor
      ORLog(kRoutine) << "Will stop after current run." << endl;
      break;
    }
    if (fgIsAHelpVariation[command]) {
      cout << fgHelp;
    }
  }
  pthread_exit(NULL);
}

int ORProcessStopper::Join()
{
  pthread_attr_destroy(&fAttr);
  //int status;
  //int retCode = pthread_join(fThread, (void**) &status);
  int retCode = pthread_join(fThread, NULL);
  if (retCode) {
    ORLog(kError) << "Join(): return code from pthread_join() is " 
                  << retCode << endl;
  }
  return retCode;
}

bool ORProcessStopper::Stop() 
{ 
  bool quitFileExists = (access("quit", F_OK) == 0); 
  if(quitFileExists) system("rm quit");
  return StopNow() || quitFileExists;
}

void ORProcessStopper::Handle(int signal)
{
  // FIXME: signal handling in threads should be handled with a signal
  // mask, and other such fancy tricks. See for example
  // http://www.scit.wlv.ac.uk/~jphb/comms/sockets.mthread.html
  // However, since this is such a simple thread structure (as of the time
  // of this writing), I'm just going to try to kill the thread. This,
  // however, calls ORVSigHandler::HandleAll(signal) to be called AGAIN,
  // and is the reason for the fgHaveHandled flags in that class that aim
  // to make sure the appropriate Handle functions are called only once.
  ORLog(kWarning) << "Handle(signal): sending " << signal << " to stopper thread" << endl;
  pthread_kill(fThread, signal);
}
