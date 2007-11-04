// ORLakeshore210Dumper.cc

#include "ORLakeshore210Dumper.hh"

#include "ORLogger.hh"

using namespace std;

ORLakeshore210Dumper::ORLakeshore210Dumper() :
ORDataProcessor(new ORLakeshore210Decoder)
{
  fLakeshore210Decoder = dynamic_cast<ORLakeshore210Decoder*>(fDataDecoder);
}

ORLakeshore210Dumper::~ORLakeshore210Dumper()
{
  delete fLakeshore210Decoder;
}

ORDataProcessor::EReturnCode ORLakeshore210Dumper::ProcessMyDataRecord(UInt_t* record)
{
  fLakeshore210Decoder->Dump(record);
  return kSuccess;
}

