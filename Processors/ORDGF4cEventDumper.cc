// ORDGF4cEventDumper.cc

#include "ORDGF4cEventDumper.hh"

#include "ORLogger.hh"

using namespace std;

ORDGF4cEventDumper::ORDGF4cEventDumper() :
ORDataProcessor(new ORDGF4cEventDecoder)
{
  fDGF4cEventDecoder = dynamic_cast<ORDGF4cEventDecoder*>(fDataDecoder);
}

ORDGF4cEventDumper::~ORDGF4cEventDumper()
{
  delete fDGF4cEventDecoder;
}

ORDataProcessor::EReturnCode ORDGF4cEventDumper::ProcessMyDataRecord(UInt_t* record)
{
  fDGF4cEventDecoder->Dump(record);
  return kSuccess;
}

