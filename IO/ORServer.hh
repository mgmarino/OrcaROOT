// ORSocket.hh

#ifndef _ORServer_hh_
#define _ORServer_hh_

#include "TServerSocket.h"
#include "ORVSigHandler.hh"

class ORServer : public TServerSocket, ORVSigHandler
{
  public:
    ORServer(int aPort);
    virtual ~ORServer(); 

    virtual void Handle(int); 
    /* Handle clean-up when ctrl-c is called. */

};

#endif
