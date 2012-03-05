// ORVXMDecoder.cc

#include "ORVXMDecoder.hh"
#include "ORLogger.hh" 
#include "ORUtils.hh"
#include "ORDictionary.hh"
#include <sstream>

void ORVXMDecoder::Dump(UInt_t* record)
{
  ORLog(kDebug) << "*************** ORVXMDecoder::Dump() *************" << std::endl;
  ORLog(kDebug) << "\tNumber of motors     : " << GetNumberOfMotors() << std::endl;
  ORLog(kDebug) << "\tTime            	   : " << GetTime(record) << std::endl;
  ORLog(kDebug) << "\tIs running      	   : " << GetIsRunning(record) << std::endl;
  ORLog(kDebug) << "\tX Position (motor 1) : " << GetXPosition(record) << std::endl;
  ORLog(kDebug) << "\tY Position (motor 2) : " << GetYPosition(record) << std::endl;
  ORLog(kDebug) << "**************************************************" << std::endl;
}