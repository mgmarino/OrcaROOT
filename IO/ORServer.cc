// ORServer.cc

#include "ORServer.hh"
#include "ORLogger.hh"

ORServer::ORServer(int aPort) : TServerSocket(aPort, kTRUE)
{
}

ORServer::~ORServer()
{
}

void ORServer::Handle(int)
{
  ORLog(kRoutine) << "Caught ctrl-c, shutting down server." << std::endl;
  Close();
}
