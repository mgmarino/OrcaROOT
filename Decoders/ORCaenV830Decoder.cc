// ORCaenV830Decoder.cc

#include "TROOT.h"
#include "ORCaenV830Decoder.hh"
#include "ORLogger.hh"

std::string ORCaenV830Decoder::GetParName(size_t iPar)
{
  switch(iPar) {
    
    // This is for ORCV:Event
    case 0: return "crate";		    
    case 1: return "card";		    
    case 2: return "chan0RollOver";	// chan0RollOver
    case 3: return "enabledMask";	
    case 4: return "header"; 		
    case 5: return "channel";		
    case 6: return "chan0Counter";	// chan0Counter
    
    default:
      ORLog(kWarning) << "GetParName(): index (" << iPar
                      << ") out of range." << std::endl;
      return "unknown";
  }
}


UInt_t ORCaenV830Decoder::IthChannelOf(UInt_t* record, size_t iRow) {
  UInt_t enabledMask = GetEnabledMask(record);
  UInt_t ch;
  iRow++;
  for(ch = 0; ch < 32 && iRow > 0; ch++)
    if(enabledMask & (1 << ch)) iRow--;
  return ch;
}

UInt_t ORCaenV830Decoder::GetPar(UInt_t* record, size_t iPar, size_t iRow) 
{
  switch(iPar) {
    /*    
    From ORCV830Decoders.m (ORCA file as used by MJD)
    Event Record
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx 			//record[0] (not needed here)
    ^^^^ ^^^^ ^^^^ ^^----------------------- V830 ID (from header)	
    -----------------^^ ^^^^ ^^^^ ^^^^ ^^^^- length (variable)
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx		        //record[1]
    --------^-^^^--------------------------- Crate number
    -------------^-^^^^--------------------- Card number
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx- Chan0 Roll over	//record[2]
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx- Enabled Mask	//record[3]
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  header	        //record[4]
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  counter 0		//record[5]
    ^  <== sign bit (this line by clint)
    ..
    ..
    xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  counter 31 // note that ONLY ENABLED CHANNELS are included so list may be shorter
    */
    
    // for debugging
    /*case 0: { std::printf("Cr:%u  Ca:%u  RO:%u  EMsk:%u  Head:%u  IthChan:%u  Call5AsInt:%u \n",
			  CrateOf(record),CardOf(record),CallAsInt(record,2),CallAsInt(record,3),
			  CallAsInt(record,4),IthChannelOf(iRow),CallHEXAsInt(record,5));
	      return CrateOf(record); }
    */
    case 0: return CrateOf(record);
    case 1: return CardOf(record);
    case 2: return GetChan0RollOver(record); //rollOver	
    case 3: return GetEnabledMask(record); //enabledMask
    case 4: return GetHeader(record); //header
    case 5: return IthChannelOf(record, iRow); //iRow stays 0 for NScalersOf set to 1
    case 6: return GetCounter(record, iRow); //Ch iRow as int
        
    // note: a "bad read" will display 4294967295 (hex: 0xffffffff, signed: -1) in ch0.
    
    default:
      ORLog(kWarning) << "GetPar(): index (" << iPar
                      << ") out of range." << std::endl;
      return 0;
  }
}

