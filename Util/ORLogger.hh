// ORLogger.hh

#ifndef _ORLOGGER_HH
#define _ORLOGGER_HH

#include <iostream>

#define ERRLINE_HACK_1(line)   #line
#define ERRLINE_HACK_2(line)   ERRLINE_HACK_1(line)

#ifdef ORLog
#undef ORLog
#endif
#define ORLog(sev) ORLogger::msg( ORLogger::sev, __FILE__ "(" ERRLINE_HACK_2(__LINE__) ")" )


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

    static ESeverity GetSeverity() { return fgSeverity; }
    static std::ostream& msg(ESeverity severity, const char* location);

    static inline void SetSeverity(ESeverity severity){ fgSeverity = severity; }

  protected:
    ORLogger() {}
    ORLogger(const ORLogger &) {}
    ~ORLogger() {}

  private:
    static std::string toString(ESeverity);

    static std::ostream* fgMyOstream;
    static std::ostream* fgMyErrstream;
    static std::ostream* fgMyNullstream;

    static ESeverity fgSeverity;
};

#endif
