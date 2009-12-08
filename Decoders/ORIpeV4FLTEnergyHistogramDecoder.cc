// ORIpeV4FLTEnergyHistogramDecoder.cc

#include "TROOT.h"
#include "ORIpeV4FLTEnergyHistogramDecoder.hh"
#include "ORLogger.hh"

bool ORIpeV4FLTEnergyHistogramDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  fDataRecord = dataRecord;
  fHistogramLength = HistogramLengthOf(dataRecord);

  ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
  // remarks for Till: LengthOf(...) is from ORVDataDecoder and is the length extracted from data record -tb-
  //TODO: need to implement IsValid() ... -tb-  if(!IsValid() || LengthOf(fDataRecord) != kBufHeadLen + fHistogramLength) {
  if(LengthOf(fDataRecord) != kBufHeadLen + fHistogramLength) {
    ORLog(kDebug) << "SetDataRecord(): data record is not valid" << std::endl;
    ORLog(kDebug) << "LengthOf(data record) : " << LengthOf(fDataRecord)
      << " kBufHeadLen + fHistogramLength: " << kBufHeadLen + fHistogramLength << std::endl;
    fDataRecord = NULL;
    fHistogramLength = -1;
    return false;
  }
  ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
  return true;
}


size_t ORIpeV4FLTEnergyHistogramDecoder::CopyHistogramData(UInt_t* histogram, 
                                                     size_t len )
//copies the histogram data to the array pointed to by
//histogram, which is of length len
{

  //ORLog(kDebug) << "Entered CopyHistogramData  ... fDataRecord="  << fDataRecord << std::endl;

  size_t wflen = HistogramLengthOf(fDataRecord);
  if (wflen == 0) return 0;
  if ((len < wflen) || (len == 0)) {
    ORLog(kWarning) << "CopyHistogramData(): destination array length is " << len 
                    << "; histogram data length is " << HistogramLengthOf(fDataRecord) << std::endl;
  }
  else len = HistogramLengthOf(fDataRecord);; 
  UInt_t* histogramData = GetHistogramDataPointer();
  for(size_t i=0;i<len;i++) 
  {
    histogram[i] = histogramData[i];  
    ORLog(kWarning) << "CopyHistogramData():   histogram[i]" <<  histogram[i] 
                    << "histogramData[i] " << histogramData[i]  << std::endl;
  }
  return len;
}




std::string ORIpeV4FLTEnergyHistogramDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "channel";
    case 3: return "readoutSec";
    case 4: return "refreshTime";
    case 5: return "firstBin";
    case 6: return "lastBin";
    case 7: return "histogramLength";
    case 8: return "maxHistogramLength";
    case 9: return "binSize";
    case 10:return "offsetEMin";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORIpeV4FLTEnergyHistogramDecoder::GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return ChannelOf(record);
    case 3: return ReadoutSecOf(record);
    case 4: return RefreshTimeOf(record);
    case 5: return FirstBinOf(record);
    case 6: return LastBinOf(record);
    case 7: return HistogramLengthOf(record);
    case 8: return MaxHistogramLengthOf(record);
    case 9: return BinSizeOf(record);
    case 10:return OffsetEMinOf(record);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}

