// ORLogger.hh

#ifndef _ORLOGGER_HH
#define _ORLOGGER_HH

#include <iostream>
#include <map>
#include <pthread.h>

#define ERRLINE_HACK_1(line)   #line
#define ERRLINE_HACK_2(line)   ERRLINE_HACK_1(line)

#ifdef ORLog
#undef ORLog
#endif
#define ORLog(sev) ORLogger::msg( pthread_self(), ORLogger::sev, __FILE__ "(" ERRLINE_HACK_2(__LINE__) ")" )
#define GetSeverity()     GetORLoggerSeverity( pthread_self() )
#define SetSeverity(sev)  SetORLoggerSeverity( pthread_self() , sev )
#define SetOStream(str)   SetORLoggerOStream( pthread_self(), str)

#include "ORReadWriteLock.hh"

//! Logging class to be used by OrcaROOT
/*!
    This class defines how to use output.  
    Usage is as follows:

    \verbatim
    ORLog(kDebug) << "This is a message" << std::endl; 

    ORLogger::SetSeverity( kDebug );  // Or some other severity
    ORLogger::SetOStream( &std::cout );  // Or some other ostream
    \endverbatim

    The logging class will pipe this to the correct output if the 
    current severity is set to above the passed-in severity.

    This class handles threads by assigning each thread it's own OStream.
    By default, this first thread to use ORLog will get the cout ostream 
    and all other threads will be piped to null output.  To avoid this, call
    ORLogger::SetOStream in each thread with a unique ostream for 
    each thread.   
 */
class ORLogger 
{
  public:
    /*!
       - kFatal:   The message is related to a condition preventing
                   further execution of the program.  ErrLogger will
                   terminate the program.  Programmers should not call
                   abort or exit themselves.
       - kError:   A condition exists such that requested result
                   or action can not be produced.  This is  serious
       - kWarning: The result is produced, but may not be
                   what's desired due to an unexpected condition
       - kRoutine: Nothing known to be wrong with the result;
                   messages that are always produced in normal
                   operation
       - kTrace:   Messages about the flow of program control
                   and which optional operations took place.
                   (This is the default if nothing is defined)
       - kDebug:   Information in addition to the above
    */
    enum ESeverity { kDebug, kTrace, kRoutine, kWarning, kError, kFatal };

    static ESeverity GetORLoggerSeverity(pthread_t thread);
    static std::ostream& msg(pthread_t thread, ESeverity severity, 
      const char* location);

    static void SetORLoggerSeverity(pthread_t thread, ESeverity severity);
    static void SetORLoggerOStream(pthread_t thread, std::ostream* aStream);

  protected:
    ORLogger() {}
    ORLogger(const ORLogger &) {}
    ~ORLogger() {}
    static std::ostream* GetORLoggerOStream(pthread_t thread);

  private:
    static bool fgIsInitialized;
    static std::string toString(ESeverity);

    static std::ostream* fgMyOstream;
    static std::ostream* fgMyNullstream;

    static std::map<pthread_t, std::pair<ESeverity, std::ostream*> > fgLoggerMap;
    static ORReadWriteLock fgRWLock;
};

#endif
