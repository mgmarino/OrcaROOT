// ORUtils.cc

#include "ORUtils.hh"

using namespace std;

// OrcaRoot assumes by default that all Orca files are written
// on PowerPC, in big endian. The user or a managing processor
// (e.g. ORDataProcManager) must SetFileIsLittleEndian(bool) externally. 
bool ORUtils::fFileIsLittleEndian = false;

bool ORUtils::SysIsLittleEndian()
{
#ifndef ORBIG_ENDIAN_MACHINE
  return true;
#else
  return false; 
#endif
}
