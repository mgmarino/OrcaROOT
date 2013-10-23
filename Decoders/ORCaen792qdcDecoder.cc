// ORCaen792qdcDecoder.cc

#include "TROOT.h"
#include "ORCaen792qdcDecoder.hh"
#include "ORLogger.hh"

using namespace std;

ORCaen792qdcDecoder::ORCaen792qdcDecoder()
{
  fLastEventCount = -1;
}

size_t ORCaen792qdcDecoder::NValuesOf(UInt_t* record)
{
  if(EventCountOf(record) != fLastEventCount) {
    LoadLocPtrs(record);
    fLastEventCount = EventCountOf(record);
  }
  return fLocPtrs.size();
}

void ORCaen792qdcDecoder::LoadLocPtrs(UInt_t* record)
{
  // loop over record and look for / skip over bad data
  fLocPtrs.clear();
  size_t i=2;
  while(i<LengthOf(record)-1) {
    if(!IthWordIsData(record, i)) {
      ORLog(kWarning) << "expected word " << i+1 << " to be a data word. Hex dump:" << endl;
      DumpHex(record);
      return;
    }
    fLocPtrs.push_back(record+i);
    i++;
  }
  // check that we end with a end-of-block word
  if(!IthWordIsEndOfBlock(record, i)) {
      ORLog(kWarning) << "expected word " << i+1 << " to be end-of-block. Hex dump:" << endl;
  }
}

UInt_t* ORCaen792qdcDecoder::GetLocPtr(UInt_t* record, size_t i)
{
  if(i >= NValuesOf(record)) {
    ORLog(kWarning) << "you asked for qdc value " << i << ", but there are only " 
                    << NValuesOf(record) << " records" << endl;
    return NULL;
  }
  return fLocPtrs[i];
}

string ORCaen792qdcDecoder::GetParName(size_t iPar)
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
                      << ") out of range." << endl;
      return "unknown";
  }
}

UInt_t ORCaen792qdcDecoder::GetPar(UInt_t* record, size_t iPar, size_t iRow)
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
                      << ") out of range." << endl;
      return 0;
  }
}

