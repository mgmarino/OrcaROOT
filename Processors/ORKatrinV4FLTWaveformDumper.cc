// ORKatrinV4FLTWaveformDumper.cc

#include "ORKatrinV4FLTWaveformDumper.hh"

#include "ORLogger.hh"

using namespace std;

ORKatrinV4FLTWaveformDumper::ORKatrinV4FLTWaveformDumper() :
ORDataProcessor(new ORKatrinV4FLTWaveformDecoder)
{
  fKatrinV4FLTWaveformDecoder = dynamic_cast<ORKatrinV4FLTWaveformDecoder*>(fDataDecoder);
}

ORKatrinV4FLTWaveformDumper::~ORKatrinV4FLTWaveformDumper()
{
  delete fKatrinV4FLTWaveformDecoder;
}

ORDataProcessor::EReturnCode ORKatrinV4FLTWaveformDumper::ProcessMyDataRecord(UInt_t* record)
{
  fKatrinV4FLTWaveformDecoder->Dump(record);
  return kSuccess;
}

