// ORVXMDecoder.cc

#include "ORVXMDecoder.hh"
#include "ORLogger.hh" 
#include "ORUtils.hh"
#include "ORDictionary.hh"
#include <sstream>

Double_t ORVXMDecoder::GetConvertedPosition(UInt_t* record)
{	
	Float_t cal_pos = GetRawPosition(record)/GetConversionFactor(record);
	return cal_pos;
}

void ORVXMDecoder::Dump(UInt_t* record)
{
  ORLog(kDebug) << "*************** ORVXMDecoder::Dump() *************" << std::endl;
  ORLog(kDebug) << "\tNumber of motors     : " << GetNumberOfMotors() << std::endl;
  ORLog(kDebug) << "\tTime            	   : " << GetTime(record) << std::endl;
  ORLog(kDebug) << "\tMotor ID        	   : " << GetMotorID(record) << std::endl;
  ORLog(kDebug) << "\tPosition (steps)     : " << GetRawPosition(record) << std::endl;
  ORLog(kDebug) << "\tPosition (mm)        : " << GetConvertedPosition(record) << std::endl;
  ORLog(kDebug) << "\tPosition (steps/mm)  : " << GetConversionFactor(record) << std::endl;
  ORLog(kDebug) << "**************************************************" << std::endl;
}