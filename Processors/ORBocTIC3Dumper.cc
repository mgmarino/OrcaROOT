// ORBocTIC3Dumper.cc

#include "ORBocTIC3Dumper.hh"

#include "ORLogger.hh"

using namespace std;

ORBocTIC3Dumper::ORBocTIC3Dumper() :
ORDataProcessor(new ORBocTIC3Decoder)
{
  fBocTIC3Decoder = dynamic_cast<ORBocTIC3Decoder*>(fDataDecoder);
}

ORBocTIC3Dumper::~ORBocTIC3Dumper()
{
  delete fBocTIC3Decoder;
}

ORDataProcessor::EReturnCode ORBocTIC3Dumper::ProcessMyDataRecord(UInt_t* record)
{
  fBocTIC3Decoder->Dump(record);
  return kSuccess;
}

