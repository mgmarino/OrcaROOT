// ORUtils.cc

#include "ORUtils.hh"

using namespace std;

// OrcaRoot assumes by default that all Orca files are written
// on PowerPC, in big endian. The user or a managing processor
// (e.g. ORDataProcManager) must SetFileIsLittleEndian(bool) externally. 
bool ORUtils::fFileIsLittleEndian = false;

bool ORUtils::SysIsLittleEndian()
{
  /*
  static bool firstTime = true;
  static bool sysIsLittleEndian = true;
  if(firstTime)
  {
    int i = 1;
    const char *p = (const char *) &i;
    if (p[0] == 1) sysIsLittleEndian=true;
    else sysIsLittleEndian=false;
    firstTime = false;
  }
  return sysIsLittleEndian;*/
#ifndef ORBIG_ENDIAN_MACHINE
  return true;
#else
  return false; 
#endif
}
