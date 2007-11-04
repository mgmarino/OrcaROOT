// ORAugerFLTWaveformDumper.cc

#include "ORAugerFLTWaveformDumper.hh"

#include "ORLogger.hh"

using namespace std;

ORAugerFLTWaveformDumper::ORAugerFLTWaveformDumper() :
ORDataProcessor(new ORAugerFLTWaveformDecoder)
{
  fAugerFLTWaveformDecoder = dynamic_cast<ORAugerFLTWaveformDecoder*>(fDataDecoder);
}

ORAugerFLTWaveformDumper::~ORAugerFLTWaveformDumper()
{
  delete fAugerFLTWaveformDecoder;
}

ORDataProcessor::EReturnCode ORAugerFLTWaveformDumper::ProcessMyDataRecord(UInt_t* record)
{
  fAugerFLTWaveformDecoder->Dump(record);
  return kSuccess;
}

