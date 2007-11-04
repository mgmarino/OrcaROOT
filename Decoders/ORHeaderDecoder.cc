// ORHeaderDecoder.cc

#include "ORHeaderDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

using namespace std;

ORHeaderDecoder::EOrcaStreamVersion ORHeaderDecoder::GetStreamVersion(UInt_t firstWordOfStream)
{
  // check this first, for speed.
  if(ORVDataDecoder::DataIdOf(&firstWordOfStream) == ExpectedDataId()) return kNewUnswapped;

  if (firstWordOfStream == FirstWordOldVersion()) return kOld;

  ORUtils::Swap(firstWordOfStream);
  if(ORVDataDecoder::DataIdOf(&firstWordOfStream) == ExpectedDataId()) return kNewSwapped;

  return kUnknownVersion;
}

bool ORHeaderDecoder::IsHeader(UInt_t firstWordOfRecord)
{
  // This function expects that the ORVReader has already determined the
  // file version when the file was first opened and has swapped the first
  // word if necessary.
  EOrcaStreamVersion fileVersion = GetStreamVersion(firstWordOfRecord);
  if(fileVersion == kOld || fileVersion == kNewUnswapped) return true;
  // We no longer support the old header.
  /*if(fileVersion == kNewUnswapped) return true;
  else if(fileVersion == kOld){
    ORLog(kWarning) << "Header not recognized.  It is either old or corrupted." << endl;
    return false;
  }*/
  else return false;
}
