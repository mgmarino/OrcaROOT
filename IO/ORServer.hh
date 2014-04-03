// ORSocket.hh

#ifndef _ORServer_hh_
#define _ORServer_hh_

#include "TServerSocket.h"
#include "ORVSigHandler.hh"

//! Implements a server.
/*!
    This class accepts a connections on a port.  Usage:

    \verbatim
    ORServer aServer( 44556 ); 
    TSocket* aSock = aServer.Accept(); 
    if ( aSock != NULL ) {
      // work with socket
    }
    \endverbatim
    
    ORServer::Accept will block until a socket is opened
    or will return NULL if it is canceled via ORVSigHandler. 

    It is important to check IsValid() before calling Accept() to
    ensure that the server is still open.
 */
class ORServer : public TServerSocket, ORVSigHandler
{
  public:
    //! Open a server listening on port: aPort
    ORServer(int aPort);
    virtual ~ORServer() { Close(); } 
    
    //! Returns a socket, and NULL if there's an error. 
    virtual TSocket* Accept(UChar_t opt = 0);

  ClassDef(ORServer, 0)

};

#endif
