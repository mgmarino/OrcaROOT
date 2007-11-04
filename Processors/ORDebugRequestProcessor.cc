// ORDebugRequestProcessor.cc

#include "ORDebugRequestProcessor.hh"

#include "TROOT.h"
#include "ORLogger.hh"

using namespace std;

ORDebugRequestProcessor::ORDebugRequestProcessor() 
{
  /* We're debugging, so basically read back what came in. */
  SetInput("DebugInt", &fInt, "An integer to debug");
  SetInput("DebugDouble", &fDouble, "A double to debug");
  SetInput("DebugString", &fString, "A string to debug");
  SetInput("DebugIntArray", &fIntVector, "An integer array to debug");
  SetInput("DebugDoubleArray", &fDoubleVector, "A double array to debug");
  SetInput("DebugStringArray", &fStringVector, "A string array to debug");
  SetOutput("DebugInt", &fInt, "An integer to debug");
  SetOutput("DebugDouble", &fDouble, "A double to debug");
  SetOutput("DebugString", &fString, "A string to debug");
  SetOutput("DebugIntArray", &fIntVector, "An integer array to debug");
  SetOutput("DebugDoubleArray", &fDoubleVector, "A double array to debug");
  SetOutput("DebugStringArray", &fStringVector, "A string array to debug");

}

bool ORDebugRequestProcessor::ExecuteProcess()
{
  ORLog(kDebug) << "Integer: " << fInt << endl;
  ORLog(kDebug) << "Double: " << fDouble << endl;
  ORLog(kDebug) << "String: " << fString << endl;
  for(size_t i=0;i<fIntVector.size();i++) {
    ORLog(kDebug) << "IntVec: " << fIntVector[i] << endl;
  }
  for(size_t i=0;i<fStringVector.size();i++) {
    ORLog(kDebug) << "StringVec: " << fStringVector[i] << endl;
  }
  for(size_t i=0;i<fDoubleVector.size();i++) {
    ORLog(kDebug) << "DoubleVec: " << fDoubleVector[i] << endl;
  }
  return true;
}

ORDebugRequestProcessor::~ORDebugRequestProcessor()
{
}



