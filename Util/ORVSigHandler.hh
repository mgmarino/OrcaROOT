// ORVSigHandler.hh

#ifndef _ORVSigHandler_hh_
#define _ORVSigHandler_hh_

#include <vector>
#include <map>

class ORVSigHandler
{
  public:
    ORVSigHandler();
    virtual ~ORVSigHandler();

    static void HandleAll(int signal);
    virtual void Handle(int signal) = 0;

  protected:

    static std::vector<ORVSigHandler*> fgHandlers;
    static std::map<int, bool> fgHaveHandled;
};

#endif
