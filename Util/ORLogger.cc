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

ORLogger::ESeverity ORLogger::fgSeverity = ORLogger::kRoutine;
ostream* ORLogger::fgMyOstream = &cout;
ostream* ORLogger::fgMyErrstream = &cerr;
ostream* ORLogger::fgMyNullstream = &gDevNull;

ostream& ORLogger::msg(ORLogger::ESeverity severity, const char* location)
{
  if (severity >= fgSeverity) {
    *fgMyOstream << toString(severity) << ": " << "(pid: " << getpid() << "): " << location << ": ";
  } else {
    return *fgMyNullstream ;
  }

  if ( severity == kFatal ){
    if (severity >= fgSeverity) *fgMyOstream << endl;
    ::abort();
  }
  return *fgMyOstream;
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
      ORLog(kWarning) << "unknown severity " << severity << ", assuming the worst..." << endl;
      return "Fatal";
  }
}

