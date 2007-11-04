// ORHeaderProcessor.cc

#include "ORHeaderProcessor.hh"

#include "TROOT.h"
#include "ORLogger.hh"

using namespace std;

ORHeaderProcessor::ORHeaderProcessor() : ORDataProcessor(new ORHeaderDecoder)
{
  fHeaderDecoder = dynamic_cast<ORHeaderDecoder*>(fDataDecoder);
  fByteCount = 0;
}

ORHeaderProcessor::~ORHeaderProcessor()
{
  delete fHeaderDecoder;
}

ORDataProcessor::EReturnCode ORHeaderProcessor::ProcessDataRecord(UInt_t* record)
{
  /* Here we explicitly check if we have at least a reasonable buffer length. */
  if (fHeaderDecoder->IsHeader(record[0])) {
    if (fHeader.LoadHeaderString(fHeaderDecoder->HeaderStringOf(record), 
      (fHeaderDecoder->NBytesOf(record)<8) ? 0 : fHeaderDecoder->NBytesOf(record)-8)) {
      return kSuccess;
    }
    else {
      return kFailure;
    }
  }
  return kFailure;
}
