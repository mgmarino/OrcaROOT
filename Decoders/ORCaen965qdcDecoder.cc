// ORCaen965qdcDecoder.cc

#include "TROOT.h"
#include "ORCaen965qdcDecoder.hh"
#include "ORLogger.hh"

#include <iostream>

ORCaen965qdcDecoder::ORCaen965qdcDecoder()
{
  fRecord = NULL;
}

size_t ORCaen965qdcDecoder::NValuesOf(UInt_t* record)
{
  if(record != fRecord) {
    LoadLocPtrs(record);
    fRecord = record;
  }
  return fLocPtrs.size();
}

void ORCaen965qdcDecoder::LoadLocPtrs(UInt_t* record)
{
  fLocPtrs.clear();
  size_t i=2;
  while(i<LengthOf(record)) {
    if(!IthWordIsData(record, i)) {
      ORLog(kWarning) << "expected word " << i+1 << " to be a data word." << std::endl;
      return;
    }
    fLocPtrs.push_back(record+i);
    i++;
    if(i > LengthOf(record)) {
      ORLog(kWarning) << "i = " << i << " is greater than the record length = " 
		      << LengthOf(record) << std::endl;
    }
  }
}

UInt_t* ORCaen965qdcDecoder::GetLocPtr(UInt_t* record, size_t i)
{
  if(i >= NValuesOf(record)) {
    ORLog(kWarning) << "you asked for qdc value " << i << ", but there are only " 
                    << NValuesOf(record) << " records" << std::endl;
    return NULL;
  }
  return fLocPtrs[i];
}

std::string ORCaen965qdcDecoder::GetParName(size_t iPar)
{
  switch(iPar) {
    case 0: return "crate";
    case 1: return "card";
    case 2: return "channel";
    case 3: return GetValueName();
    case 4: return "underthresh";
    case 5: return "overflow";
    case 6: return "isValid";
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}

UInt_t ORCaen965qdcDecoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
{
  switch(iPar) {
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return IthChannelOf(record, iRow);
    case 3: return IthValueOf(record, iRow);
    case 4: return IthValueIsUnderThreshold(record, iRow);
    case 5: return IthValueIsOverflow(record, iRow);
    case 6: return IthValueIsValid(record, iRow);
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}