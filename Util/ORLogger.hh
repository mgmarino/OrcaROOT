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
#define GetOStream()      GetORLoggerOStream( pthread_self() )
#define SetSeverity(sev)  SetORLoggerSeverity( pthread_self() , ORLogger::sev )
#define SetOStream(str)   SetORLoggerOStream( pthread_self(), str)

class ORLogger 
{
  public:
    enum ESeverity { kDebug, kTrace, kRoutine, kWarning, kError, kFatal };
    // kFatal:   The message is related to a condition preventing
    //           further execution of the program.  ErrLogger will
    //           terminate the program.  Programmers should not call
    //           abort or exit themselves.
    // kError:   A condition exists such that requested result
    //           or action can not be produced.  This is  serious
    // kWarning: The result is produced, but may not be
    //           what's desired due to an unexpected condition
    // kRoutine: Nothing known to be wrong with the result;
    //           messages that are always produced in normal
    //           operation
    // kTrace:   Messages about the flow of program control
    //           and which optional operations took place.
    //           (This is the default if nothing is defined)
    // kDebug:   Information in addition to the above

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
    static pthread_rwlock_t fgRWLock;
    static void Initialize();
};

#endif
