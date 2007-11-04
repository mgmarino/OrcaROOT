// ORProcessStopper.hh

#ifndef _ORProcessStopper_hh_
#define _ORProcessStopper_hh_

#include <map>
#include <pthread.h>
#include <string>
#include "ORVSigHandler.hh"

class ORProcessStopper : public ORVSigHandler
{
  public:
    ORProcessStopper();
    virtual ~ORProcessStopper();

    virtual int ExecuteStopperThread();
    static void* StopperThread(void* /*arg*/);
    static void ShowHelp();
    static void Lock() { pthread_mutex_lock(&fgStopMutex); }
    static void Unlock() { pthread_mutex_unlock(&fgStopMutex); }
    virtual int Join();
    static bool StopNow() { Lock(); bool tmp = fgStopNow; Unlock(); return tmp; }
    static bool Stop();

    virtual void Handle(int signal);

  protected:
    static bool fgStopNow;
    static pthread_mutex_t fgStopMutex;
    static std::map<std::string, bool> fgIsAQuitVariation;
    static std::map<std::string, bool> fgIsAQuitNowVariation;
    static std::map<std::string, bool> fgIsAHelpVariation;
    static const char fgHelp[];
    pthread_t fThread;
    pthread_attr_t fAttr;
};

#endif
