// ORPyDataProcManager.hh

#ifndef _ORPyDataProcManager_hh_
#define _ORPyDataProcManager_hh_

#include "ORPyBinder.hh"
#include "ORDataProcManager.hh"

class ORPyDataProcManager : public ORDataProcManager, public ORPyBinder
{

    ClassDef(ORPyDataProcManager, 0)
};

#endif
