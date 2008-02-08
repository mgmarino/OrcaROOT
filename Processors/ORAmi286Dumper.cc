// ORAmi286Dumper.cc

#include "ORAmi286Dumper.hh"

#include "ORLogger.hh"

using namespace std;

ORAmi286Dumper::ORAmi286Dumper() :
ORDataProcessor(new ORAmi286Decoder)
{
  fAmi286Decoder = dynamic_cast<ORAmi286Decoder*>(fDataDecoder);
}

ORAmi286Dumper::~ORAmi286Dumper()
{
  delete fAmi286Decoder;
}

ORDataProcessor::EReturnCode ORAmi286Dumper::ProcessMyDataRecord(UInt_t* record)
{
  fAmi286Decoder->Dump(record);
  return kSuccess;
}

