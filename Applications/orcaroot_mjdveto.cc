#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h> 
#include <set> 

#include "ORDataProcManager.hh"
#include "ORFileReader.hh"
#include "ORFileWriter.hh"
#include "ORLogger.hh"
#include "ORSocketReader.hh"

#include "OROrcaRequestProcessor.hh"
#include "ORServer.hh"
#include "ORHandlerThread.hh"

#include "ORCaen792NqdcDecoder.hh"
#include "ORBasicTreeWriter.hh"

using namespace std;

static const char Usage[] =
"\n"
"\n"
"Usage: orcaroot [options] [input file(s) / socket host:port]\n"
"\n"
"The one required argument is either the name of a file to be processed or\n"
"a host and port of a socket from which to read data. For a file, you may\n"
"enter a series of files to be processed, or use a wildcard like \"file*.dat\"\n"
"For a socket, the argument should be formatted as host:port.\n"
"\n"
"Available options:\n"
"  --help : print this message and exit\n"
"  --verbosity [verbosity] : set the severity/verbosity for the logger.\n"
"    Choices are: debug, trace, routine, warning, error, and fatal.\n"
"  --label [label] : use [label] as prefix for root output file name.\n"
"  --keepalive [time] : keep a socket connection alive if lost.\n"
"    Wait [time] (default 10) seconds between connection attempts.\n"
"  --maxreconnect [num] : Maximum [num] socket reconnect attempts.\n"
"    A [num] value of 0 sets this to infinity (i.e. no timeout).\n"
"  --daemon [port] : Runs as a server accepting connections on [port]. \n" 
"  --connections [num] : Maximum [num] connections accepted by server. \n" 
"\n"
"Example usage:\n"
"orcaroot run194ecpu\n"
"  Rootify local file run194ecpu with default verbosity, output file label, etc.\n"
"orcaroot run*\n"
"  Same as before, but run over all files beginning with \"run\".\n"
"orcaroot --verbosity debug --label mylabel run194ecpu\n"
"  The same, but with example usage of the verbosity and mylabel options.\n"
"  An output file will be created with name mylabel_run194.root, and lots\n"
"  of debugging output will appear.\n"
"orcaroot 128.95.100.213:44666\n"
"  Rootify orca stream on host 128.95.100.213, port 44666 with default verbosity,\n"
"  output file label, etc.\n"
"orcaroot --daemon 9090 --connections 10\n"
"  Start orcaroot as a server on port 9090, accepting a maximum number of 10 connections.\n" 
"\n"
"\n";


int main(int argc, char** argv)
{
  if(argc == 1) {
    ORLog(kError) << "You must supply some options" << endl << Usage;
    return 1;
  }


  static struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"verbosity", required_argument, 0, 'v'},
    {"label", required_argument, 0, 'l'},
    {"keepalive", optional_argument, 0, 'k'},
    {"maxreconnect", required_argument, 0, 'm'},
    {"daemon", required_argument, 0, 'd'},
    {"connections", required_argument, 0, 'c'}
  };

  string label = "OR";
  ORVReader* reader = NULL;

  bool keepAliveSocket = false;
  bool runAsDaemon = false;
  unsigned long timeToSleep = 10; //default sleep time for sockets.
  unsigned int reconnectAttempts = 0; // default reconnect tries for sockets.
  unsigned int portToListenOn = 0;
  unsigned int maxConnections = 5; // default connections accepted by server

  while(1) {
    char optId = getopt_long(argc, argv, "", longOptions, NULL);
    if(optId == -1) break;
    switch(optId) {
      case('h'): // help
        cout << Usage;
        return 0;
      case('v'): // verbosity
        if(strcmp(optarg, "debug") == 0) ORLogger::SetSeverity(ORLogger::kDebug);
        else if(strcmp(optarg, "trace") == 0) ORLogger::SetSeverity(ORLogger::kTrace);
        else if(strcmp(optarg, "routine") == 0) ORLogger::SetSeverity(ORLogger::kRoutine);
        else if(strcmp(optarg, "warning") == 0) ORLogger::SetSeverity(ORLogger::kWarning);
        else if(strcmp(optarg, "error") == 0) ORLogger::SetSeverity(ORLogger::kError);
        else if(strcmp(optarg, "fatal") == 0) ORLogger::SetSeverity(ORLogger::kFatal);
        else {
          ORLog(kWarning) << "Unknown verbosity setting " << optarg 
                          << "; using kRoutine" << endl;
          ORLogger::SetSeverity(ORLogger::kRoutine);
        }
        break;
      case('l'): // label
        label = optarg;
        break;
      case('k'):
        if(!optarg) keepAliveSocket = true;
        else {
          keepAliveSocket = true;
          timeToSleep = abs(atol(optarg));
        }
        break;
      case('m'):
        reconnectAttempts = abs(atoi(optarg));
        break;
      case('d'):
        reconnectAttempts = 0;
        keepAliveSocket = false;
        runAsDaemon = true;
        portToListenOn = abs(atol(optarg));
        break;
      case('c'):
        maxConnections = abs(atoi(optarg));
        break;
      default: // unrecognized option
        ORLog(kError) << Usage;
        return 1;
    }
  }

  if (argc <= optind && !runAsDaemon) {
    ORLog(kError) << "You must supply a filename or socket host:port" << endl
                  << Usage << endl;
    return 1;
  }

  ORHandlerThread* handlerThread = new ORHandlerThread();
  handlerThread->StartThread();
  /***************************************************************************/
  /*   Running orcaroot as a daemon server. */
  /***************************************************************************/
  if (runAsDaemon) {
    /* Here we start listening on a socket for connections. */
    /* We are doing this very simply with a simple fork. Eventually want
       to check number of spawned processes, etc.  */
    ORLog(kRoutine) << "Running orcaroot as daemon on port: " << portToListenOn << endl;
    pid_t childpid = 0;
    std::set<pid_t> childPIDRecord;
    
    ORServer* server = new ORServer(portToListenOn);
    /* Starting server, binding to a port. */
    if (!server->IsValid()) {
      ORLog(kError) << "Error listening on port " << portToListenOn 
        << endl << "Error code: " << server->GetErrorCode() << endl;
      return 1;
    }
    while (1) {
      /* This while loop is broken by a kill signal which is well handled
       * by the server.  The kill signal will automatically propagate to the
       * children so we really don't have to worry about waiting for them to
       * die.  */
      while (childPIDRecord.size() >= maxConnections) { 
        /* We've reached our maximum number of child processes. */
        /* Wait for a process to end. */
        childpid = wait3(0, WUNTRACED, 0);
        if(childPIDRecord.erase(childpid) != 1) {
          /* Something really weird happened. */
          ORLog(kError) << "Ended child process " << childpid 
            << " not recognized!" << endl;
        }
      }
      while((childpid = wait3(0,WNOHANG,0)) > 0) {
        /* Cleaning up any children that may have ended.                   * 
         * This will just go straight through if no children have stopped. */
        if(childPIDRecord.erase(childpid) != 1) {
          /* Something really weird happened. */
          ORLog(kError) << "Ended child process " << childpid 
            << " not recognized!" << endl;
        }
      } 
      ORLog(kRoutine) << childPIDRecord.size()  << " connections running..." << endl;
      ORLog(kRoutine) << "Waiting for connection..." << endl;
      TSocket* sock = server->Accept(); 
      if (sock == (TSocket*) 0 || sock == (TSocket*) -1 ) {
        // There was an error, or the socket got closed .
        if (!server->IsValid()) return 0;
        continue;
      }
      if(!sock->IsValid()) {
        /* Invalid socket, cycle to wait. */
        delete sock;
        continue;
      }
      if ((childpid = fork()) == 0) {
        /* We are in the child process.  Set up reader and fire away. */
        delete server;
        delete handlerThread;
        handlerThread = new ORHandlerThread;
        handlerThread->StartThread();
        reader = new ORSocketReader(sock, true);
        /* Get out of the while loop */
        break;
      } 
      /* Parent process: wait for next connection. Close our descriptor. */
      ORLog(kRoutine) << "Connection accepted, child process begun with pid: " 
        << childpid << endl;
      childPIDRecord.insert(childpid);
      delete sock; 
    }
  /***************************************************************************/
  /*  End daemon server code.  */
  /***************************************************************************/
  } else {
    /* Normal running, either connecting to a server or reading in a file. */
    string readerArg = argv[optind];
    size_t iColon = readerArg.find(":");
    if (iColon == string::npos) {
      reader = new ORFileReader;
      for (int i=optind; i<argc; i++) {
        ((ORFileReader*) reader)->AddFileToProcess(argv[i]);
      }
    } else {
      reader = new ORSocketReader(readerArg.substr(0, iColon).c_str(), 
                                  atoi(readerArg.substr(iColon+1).c_str()));
      //((ORSocketReader*)reader)->SetKeepAlive(keepAliveSocket);
      //((ORSocketReader*)reader)->SetSleepTime(timeToSleep);
      //((ORSocketReader*)reader)->SetReconnectAttempts(reconnectAttempts);
      /*
      if(keepAliveSocket) {
        ostringstream tempString;
        tempString << reconnectAttempts;
        ORLog(kRoutine) << "Setting socket to stay alive: " << endl
          << "Sleep time: " << timeToSleep << " seconds" << endl
          << "Reconnect attempts: " 
          << ((reconnectAttempts == 0) ? "infinite" : tempString.str())
          << endl;
      }*/
    }
  }
  if (!reader->OKToRead()) {
    ORLog(kError) << "Reader couldn't read" << endl;
    return 1;
  }

  ORLog(kRoutine) << "Setting up data processing manager..." << endl;
  ORDataProcManager dataProcManager(reader);

  /* Declare processors here. */
  ORFileWriter fileWriter(label);

  ORCaen792NqdcDecoder caen792NqdcDecoder;
  ORBasicTreeWriter caen792NTreeWriter(&caen792NqdcDecoder, "vetoTree");

  OROrcaRequestProcessor orcaReq;
  if (runAsDaemon) {
    /* Add them here if you wish to run them in daemon mode ( not likely ).*/
    dataProcManager.SetRunAsDaemon();
    dataProcManager.AddProcessor(&orcaReq);
  } else {
    /* Add the processors here to run them in normal mode. */
    dataProcManager.AddProcessor(&fileWriter);
    dataProcManager.AddProcessor(&caen792NTreeWriter);
  }

  ORLog(kRoutine) << "Start processing..." << endl;
  dataProcManager.ProcessDataStream();
  ORLog(kRoutine) << "Finished processing..." << endl;

  delete reader;
  delete handlerThread;

  return 0;
}

