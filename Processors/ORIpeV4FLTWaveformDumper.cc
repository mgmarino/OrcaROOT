// ORIpeV4FLTWaveformDumper.cc

#include "ORIpeV4FLTWaveformDumper.hh"

#include "ORLogger.hh"

using namespace std;

ORIpeV4FLTWaveformDumper::ORIpeV4FLTWaveformDumper() :
ORDataProcessor(new ORIpeV4FLTWaveformDecoder)
{
  fIpeV4FLTWaveformDecoder = dynamic_cast<ORIpeV4FLTWaveformDecoder*>(fDataDecoder);
}

ORIpeV4FLTWaveformDumper::~ORIpeV4FLTWaveformDumper()
{
  delete fIpeV4FLTWaveformDecoder;
}

ORDataProcessor::EReturnCode ORIpeV4FLTWaveformDumper::ProcessMyDataRecord(UInt_t* record)
{
  fIpeV4FLTWaveformDecoder->Dump(record);
  return kSuccess;
}

