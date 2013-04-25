// ORKatrinV4FLTHitRateDecoder.cc

#include "TROOT.h"
#include "ORKatrinV4FLTHitRateDecoder.hh"
#include "ORLogger.hh"

bool ORKatrinV4FLTHitRateDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  //TODO: here I could cross check the length ... -tb-
 
  ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
  return true;
}
 
 
std::string ORKatrinV4FLTHitRateDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
  case 0: return "crate";
  case 1: return "card";
  case 2: return "seconds";
  case 3: return "hitRateLength";
  case 4: return "totalHitRate";
  case 5: return "nChannels";
  case 6: return "version";
    //  case 6: return "channel";
    //  case 7: return "overflow";
    //  case 8: return "hitRate";
  default:
    ORLog(kWarning) << "GetParName(): index (" << iPar
		    << ") out of range." << std::endl;
    return "unknown";
  }
}

UInt_t ORKatrinV4FLTHitRateDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
  case 0: return CrateOf(record);
  case 1: return CardOf(record);
  case 2: return SecondsOf(record);
  case 3: return HitRateLengthOf(record);
  case 4: return TotalHitRateOf(record);
  case 5: return NChannelsOf(record);
  case 6: return VersionOf(record);
  default:
    ORLog(kWarning) << "GetPar(): index (" << iPar
		    << ") out of range." << std::endl;
    return 0;
  }
}

size_t ORKatrinV4FLTHitRateDecoder::CopyRawData(UInt_t* data16, UInt_t* data32 /*=0*/)
{
  UInt_t fNChannels= NChannelsOf(fDataRecord);
  UInt_t version   = VersionOf(fDataRecord);
  
  //fill arrays
  for(size_t i=0;i<fNChannels;i++) data16[i] = fDataRecord[i+kHeaderSize];
  if(data32){
      if(version==0){
          for(size_t i=0;i<fNChannels;i++)  data32[i] = 0; 
      }else{
          for(size_t i=0;i<fNChannels;i++)  data32[i] = fDataRecord[i+fNChannels+kHeaderSize];
      }
  }

  
  return fNChannels;
}
