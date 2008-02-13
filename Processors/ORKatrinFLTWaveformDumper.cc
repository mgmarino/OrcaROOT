// ORKatrinFLTWaveformDumper.cc

#include "ORKatrinFLTWaveformDumper.hh"

#include "ORLogger.hh"

using namespace std;

ORKatrinFLTWaveformDumper::ORKatrinFLTWaveformDumper() :
ORDataProcessor(new ORKatrinFLTWaveformDecoder)
{
  fKatrinFLTWaveformDecoder = dynamic_cast<ORKatrinFLTWaveformDecoder*>(fDataDecoder);
}

ORKatrinFLTWaveformDumper::~ORKatrinFLTWaveformDumper()
{
  delete fKatrinFLTWaveformDecoder;
}

ORDataProcessor::EReturnCode ORKatrinFLTWaveformDumper::ProcessMyDataRecord(UInt_t* record)
{
  fKatrinFLTWaveformDecoder->Dump(record);
  return kSuccess;
}

