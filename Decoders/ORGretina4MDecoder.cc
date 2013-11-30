// ORGretaDecoder.cc

#include "ORGretina4MDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

using namespace std;

ORGretina4MDecoder::ORGretina4MDecoder() 
{
}

bool ORGretina4MDecoder::SetDataRecord(UInt_t* dataRecord) 
{
  if(IsShort(dataRecord)) {
    ORLog(kDebug) << "SetDataRecord(): data record is short form, invalid" << endl;
    fDataRecord = NULL;
    return false;
  }
  fDataRecord = dataRecord;
  return true;
}

void ORGretina4MDecoder::SetDecoderDictionary(const ORDecoderDictionary* dict)
{
  ORVDataDecoder::SetDecoderDictionary(dict);
  for(UInt_t iPar = 0; iPar < kNPars; iPar++) fCardPars[iPar].clear();
}

UInt_t ORGretina4MDecoder::GetCardParameter(ECardPars par, UInt_t crate, UInt_t card)
{
  UInt_t ccc = (crate << 12) + (card << 4); // 0xfffff000 + 0xff0
  return GetParameter(UInt_t(par), ccc);
}

UInt_t ORGretina4MDecoder::GetChannelParameter(EChanPars par, UInt_t crate, UInt_t card, UInt_t channel)
{
  UInt_t ccc = (crate << 12) + (card << 4) + channel; // 0xfffff000 + 0xff0 + 0xf
  return GetParameter(UInt_t(par), ccc);
}

UInt_t ORGretina4MDecoder::GetParameter(UInt_t par, UInt_t ccc)
{
  map<UInt_t, UInt_t>::iterator iter = fCardPars[par].find(ccc);

  //return if found
  if(iter != fCardPars[par].end()) return iter->second;

  // otherwise we have to look up the info from the header
  // first find a key
  const char* key = "";
  if(par == kIntTime) key = "Integration Time";
  //if(par == kChPreSum) key = "Chpsrt";
  if(par == kChPreSum) key = "Enabled";
  if(par == kChPreSum) key = "Mrpsrt";
  else {
    ORLog(kError) << "Unknown card parameter " << par << endl;
    return (UInt_t) -1;
  }

  // now pull out the value for the key
  UInt_t cr = ccc >> 12;
  UInt_t ca = (ccc & 0xff0) >> 4;
  UInt_t ch = (ccc & 0xf);
  UInt_t value = 0;
  if(par < kNCardPars) value = GetIntValueFromKey(key, cr, ca);
  else value = dynamic_cast<const ORDictValueI*>(GetArrayFromKey(key, cr, ca)->At(ch))->GetI();

  // cache locally and return
  fCardPars[par][ccc] = value;
  return value;
}

ULong64_t ORGretina4MDecoder::GetTimeStamp(UInt_t* header)
{
  ULong64_t time = header[3] & 0xffff;
  time = time << 32;
  time += header[2];
  return time;
}

UInt_t ORGretina4MDecoder::GetEnergy(UInt_t* header)
{ 
  UInt_t energy = header[3] >> 16;
  energy += (header[4] & 0x000001ff) << 16;
  //energy is in 2's complement, take abs value if necessary
  if (energy & 0x1000000) energy = (~energy & 0x1ffffff) + 1;
  return energy;
}

