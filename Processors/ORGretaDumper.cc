// ORGretaDumper.cc

#include "ORGretaDumper.hh"

#include "ORLogger.hh"

using namespace std;

ORGretaDumper::ORGretaDumper() :
ORDataProcessor(new ORGretaDecoder)
{
  fEventDecoder = dynamic_cast<ORGretaDecoder*>(fDataDecoder);
}

ORGretaDumper::~ORGretaDumper()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORGretaDumper::ProcessMyDataRecord(UInt_t* record)
{
  fEventDecoder->Dump(record);
  return kSuccess;
}

