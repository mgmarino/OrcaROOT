// ORServer.cc

#include "ORServer.hh"

ClassImp(ORServer)

ORServer::ORServer(int aPort) : TServerSocket(aPort, kTRUE)
{
  /* Setup no blocking. */
  SetOption(kNoBlock,1);
}

TSocket* ORServer::Accept(UChar_t opt) 
{
  TSocket* aSocket = NULL;
  while (aSocket == NULL || aSocket == (TSocket*)-1) {
    if (TestCancel()) {
      Close();
      return NULL;
    }
    aSocket = TServerSocket::Accept(opt); 
    sleep(1); // wait until we try again
  }
  return aSocket;
}


