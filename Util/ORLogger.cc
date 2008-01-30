// ORLogger.cc

#include "ORLogger.hh"

#include <sstream>

using namespace std;

//static ostringstream gDevNull;
static struct nullstream: std::ostream {
    struct nullbuf: std::streambuf {
      int overflow(int c) { return traits_type::not_eof(c); }
    } m_sbuf;
    nullstream(): std::ios(&m_sbuf), std::ostream(&m_sbuf) {}
  } gDevNull;

ostream* ORLogger::fgMyOstream = &cout;
ostream* ORLogger::fgMyNullstream = &gDevNull;
bool ORLogger::fgIsInitialized = false;
std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> > ORLogger::fgLoggerMap;
pthread_rwlock_t ORLogger::fgRWLock;

void ORLogger::Initialize()
{
  pthread_rwlock_init(&ORLogger::fgRWLock, NULL);
  fgIsInitialized = true;
}

ostream& ORLogger::msg(pthread_t thread, ORLogger::ESeverity severity, const char* location)
{
  if (!fgIsInitialized) Initialize();
  static ostream* theThreadStream;
  ORLogger::ESeverity theThreadSeverity;

  /* critical part */
  pthread_rwlock_rdlock(&fgRWLock);
  std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream* > >::iterator anIter = fgLoggerMap.find(thread);
  if (anIter != fgLoggerMap.end()) {
    theThreadStream = anIter->second.second;
    theThreadSeverity = anIter->second.first;
    pthread_rwlock_unlock(&fgRWLock);
  } else {
    pthread_rwlock_unlock(&fgRWLock);
    theThreadSeverity = GetORLoggerSeverity(thread);
    theThreadStream = fgMyOstream; 
  }
  /* end critical part. */
  if (severity >= theThreadSeverity) {
    *theThreadStream << toString(severity) << ": " << "(pid: " << getpid() << "): " << location << ": ";
  } else {
    return *fgMyNullstream ;
  }

  if ( severity == kFatal ){
    if (severity >= theThreadSeverity) *theThreadStream << endl;
    //::abort();  
    /* When we're striving for safety, abort takes down the whole shebang! */
    pthread_exit((void *) 0);
  }
  return *theThreadStream;
}

ORLogger::ESeverity ORLogger::GetORLoggerSeverity(pthread_t thread) 
{
  if (!fgIsInitialized) Initialize();
  ORLogger::ESeverity theSeverity;
  pthread_rwlock_rdlock(&fgRWLock);
  std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream* > >::iterator anIter = fgLoggerMap.find(thread);
  if ( anIter != fgLoggerMap.end() ) {
    theSeverity = anIter->second.first; 
    pthread_rwlock_unlock(&fgRWLock);
  } else {
    /* Insert with default severity, ostream. */ 
    pthread_rwlock_unlock(&fgRWLock);
    pthread_rwlock_wrlock(&fgRWLock);
    fgLoggerMap.insert(
      std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
      std::pair<ORLogger::ESeverity, std::ostream*>(ORLogger::kRoutine, fgMyOstream)));
    pthread_rwlock_unlock(&fgRWLock);
    theSeverity = ORLogger::kRoutine;
  } 
  return theSeverity;
}

void ORLogger::SetORLoggerOStream(pthread_t thread, std::ostream* aStream)
{
  if (!fgIsInitialized) Initialize();
  pthread_rwlock_wrlock(&fgRWLock);
  std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream* > >::iterator anIter = fgLoggerMap.find(thread);
  if ( anIter != fgLoggerMap.end() ) {
    anIter->second.second = aStream; 
  } else {
    /* Insert with default severity. */ 
    fgLoggerMap.insert(
      std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
      std::pair<ORLogger::ESeverity, std::ostream*>(ORLogger::kRoutine, aStream)));
  }
  pthread_rwlock_unlock(&fgRWLock);
}


void ORLogger::SetORLoggerSeverity(pthread_t thread, ORLogger::ESeverity severity)
{
  if (!fgIsInitialized) Initialize();
  pthread_rwlock_wrlock(&fgRWLock);
  std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream* > >::iterator anIter = fgLoggerMap.find(thread);
  if ( anIter != fgLoggerMap.end()) {
    anIter->second.first = severity; 
  } else {
    /* Insert with default ostream. */ 
    fgLoggerMap.insert(
      std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
      std::pair<ORLogger::ESeverity, std::ostream*>(severity, fgMyOstream)));
  }
  pthread_rwlock_unlock(&fgRWLock);
}

string ORLogger::toString(ORLogger::ESeverity severity)
{
  switch (severity) {
    case kDebug: return "Debug";
    case kTrace: return "Trace";
    case kRoutine: return "Routine";
    case kWarning: return "Warning";
    case kError: return "Error";
    case kFatal: return "Fatal";
    default:
      //ORLog(kWarning) << "unknown severity " << severity << ", assuming the worst..." << endl;
      return "Fatal";
  }
}

