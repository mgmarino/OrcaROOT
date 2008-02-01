// ORSocket.hh

#ifndef _ORServer_hh_
#define _ORServer_hh_

#include "TServerSocket.h"
#include "ORVSigHandler.hh"

class ORServer : public TServerSocket, ORVSigHandler
{
  public:
    ORServer(int aPort);
    virtual ~ORServer() { Close(); } 
    
    virtual TSocket* Accept(UChar_t opt = 0);
    /* Returns a socket, and NULL if there's an error. */

};

#endif
