// ORRunNotesProcessor.hh
// Object saves run notes in a file.  It saves the run notes in a
// TDirectoryFile as TObjStrings.  This allows these to be accessed by anyone
// opening the ROOT file.  M. Marino

#ifndef _ORRunNotesProcessor_hh_
#define _ORRunNotesProcessor_hh_

#include "ORUtilityProcessor.hh"


class ORRunNotesProcessor : public ORUtilityProcessor
{
  public:
    ORRunNotesProcessor() {}

    // overloaded from ORDataProcessor
    virtual EReturnCode StartRun();

};

#endif
