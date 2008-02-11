// ORLogger.cc

#include "ORLogger.hh"

#include <sstream>

using namespace std;

/* This is not thread-safe, but it doesn't matter what gets written to this when. */
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
ORReadWriteLock ORLogger::fgRWLock;

ostream& ORLogger::msg(pthread_t thread, ORLogger::ESeverity severity, const char* location)
{
  ostream* theThreadStream;
  ORLogger::ESeverity theThreadSeverity;

  /* critical part */
  fgRWLock.readLock();
  std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream* > >::iterator anIter = fgLoggerMap.find(thread);
  if (anIter != fgLoggerMap.end()) {
    theThreadStream = anIter->second.second;
    theThreadSeverity = anIter->second.first;
    fgRWLock.unlock();
  } else {
    fgRWLock.unlock();
    theThreadSeverity = ORLogger::kRoutine;
    theThreadStream = GetORLoggerOStream(thread); 
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

std::ostream* ORLogger::GetORLoggerOStream(pthread_t thread) 
{
  ostream* theThreadStream;
  fgRWLock.readLock();
  std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream* > >::iterator anIter = fgLoggerMap.find(thread);

  if ( anIter != fgLoggerMap.end() ) {
    theThreadStream = anIter->second.second; 
    fgRWLock.unlock();
  } else {
    /* Insert with default severity, ostream. */ 
    fgRWLock.unlock();
    fgRWLock.writeLock();
    if (fgLoggerMap.size() > 0) {
      /* If the map already has one, then the others get /dev/null */
      fgLoggerMap.insert(
        std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
        std::pair<ORLogger::ESeverity, std::ostream*>(ORLogger::kRoutine, fgMyNullstream)));
      theThreadStream = fgMyNullstream;
    } else {
      fgLoggerMap.insert(
        std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
        std::pair<ORLogger::ESeverity, std::ostream*>(ORLogger::kRoutine, fgMyOstream)));
      theThreadStream = fgMyOstream;
    }
    fgRWLock.unlock();
  } 
  return theThreadStream;
}

ORLogger::ESeverity ORLogger::GetORLoggerSeverity(pthread_t thread) 
{
  ORLogger::ESeverity theSeverity;
  fgRWLock.readLock();
  std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream* > >::iterator anIter = fgLoggerMap.find(thread);
  if ( anIter != fgLoggerMap.end() ) {
    theSeverity = anIter->second.first; 
    fgRWLock.unlock();
  } else {
    /* Insert with default severity, ostream. */ 
    fgRWLock.unlock();
    fgRWLock.writeLock();
    if (fgLoggerMap.size() > 0) {
      /* If the map already has one, then the others get /dev/null */
      fgLoggerMap.insert(
        std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
        std::pair<ORLogger::ESeverity, std::ostream*>(ORLogger::kRoutine, fgMyNullstream)));
    } else {
      fgLoggerMap.insert(
        std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
        std::pair<ORLogger::ESeverity, std::ostream*>(ORLogger::kRoutine, fgMyOstream)));
    }
    fgRWLock.unlock();
    theSeverity = ORLogger::kRoutine;
  } 
  return theSeverity;
}

void ORLogger::SetORLoggerOStream(pthread_t thread, std::ostream* aStream)
{
  if (aStream == NULL) aStream = fgMyNullstream;
  fgRWLock.writeLock();
  std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream* > >::iterator anIter = fgLoggerMap.find(thread);
  if ( anIter != fgLoggerMap.end() ) {
    anIter->second.second = aStream; 
  } else {
    /* Insert with default severity. */ 
    fgLoggerMap.insert(
      std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
      std::pair<ORLogger::ESeverity, std::ostream*>(ORLogger::kRoutine, aStream)));
  }
  fgRWLock.unlock();
}


void ORLogger::SetORLoggerSeverity(pthread_t thread, ORLogger::ESeverity severity)
{
  fgRWLock.writeLock();
  std::map<pthread_t, std::pair<ORLogger::ESeverity, std::ostream* > >::iterator anIter = fgLoggerMap.find(thread);
  if ( anIter != fgLoggerMap.end()) {
    anIter->second.first = severity; 
  } else {
    /* Insert with default ostream. */ 
    if (fgLoggerMap.size() > 0) {
      /* If the map already has one, then the others get /dev/null */
      fgLoggerMap.insert(
        std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
        std::pair<ORLogger::ESeverity, std::ostream*>(severity, fgMyNullstream)));
    } else {
      fgLoggerMap.insert(
        std::pair< pthread_t, std::pair<ORLogger::ESeverity, std::ostream*> >(thread, 
        std::pair<ORLogger::ESeverity, std::ostream*>(severity, fgMyOstream)));
    }
  }
  fgRWLock.unlock();
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

