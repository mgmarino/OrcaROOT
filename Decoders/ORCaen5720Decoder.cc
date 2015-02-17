// ORCaen5720Decoder.cc
// Basic information taken from ORCaen1720Decoder.cc provided by Jarek Kaspar.  
// Updated by Laura Bodine
// Zpack2.5 and zerolengthencoding are not currently handled here...
// if your data is formatted that way then you must add that functionality to this decoder

#include "TROOT.h"
#include "ORCaen5720Decoder.hh"
#include "ORLogger.hh"



/**************************************************************************
// Get number of samples in each trace (single channel)
**************************************************************************/
UInt_t ORCaen5720Decoder::TraceLength(UInt_t* record) {
	UInt_t numChan = 0;
	UInt_t chanMask = ChannelMask(record);

	for (; chanMask; numChan++) chanMask &= chanMask - 1; //check number of channels in record

	return (EventSize(record) - 4) / numChan * 2; // 4 longs header, then 2 samples per long
}


/**************************************************************************
//Copy the trace for the first ACTIVE channel.  Check ChannelMap to tell.  
**************************************************************************/

void ORCaen5720Decoder::CopyTrace(UInt_t* record, UInt_t* Waveform, UInt_t numSamples) {
         UInt_t * evtpointer = GetEventPointer(record);
         UShort_t* sample = (UShort_t*) evtpointer;
	//iterative way, because of the coming packed2.5 mode
	for (unsigned int i = 0; i < numSamples; i++) {
	  Waveform[i] =  sample[i+8] & 0x0fff;
	  //std::cout << "Waveform [" << i << "] = " << Waveform[i] << std::endl;       
	}
}

/**************************************************************************
// Get at all traces (array arithmetic based on ChannelMask active channels)  
**************************************************************************/

void ORCaen5720Decoder::CopyTraces(UInt_t* record, UInt_t *Waveform0, UInt_t *Waveform1,UInt_t *Waveform2,UInt_t *Waveform3, UInt_t numSamples){
         UInt_t * evtpointer = GetEventPointer(record);
         UShort_t* sample = (UShort_t*) evtpointer;

	 UInt_t chanMask = ChannelMask(record); //chanMask: int between 1 and 15

	 switch(chanMask){
	   case 1 :
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform0[i] = sample[i+8] & 0x0fff;
	     };
	     break;
	     
	   case 2 :
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform1[i] = sample[i+8] & 0x0fff;
	     };
	     break;

	   case 3:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform0[i] = sample[i+8] & 0x0fff;
	       Waveform1[i] = sample[i+8+numSamples] & 0x0fff;
	     };
	     break;

	   case 4:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform2[i] = sample[i+8] & 0x0fff;
	     };
	     break;

	   case 5:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform0[i] = sample[i+8] & 0x0fff;
	       Waveform2[i] = sample[i+8+numSamples] & 0x0fff;
	     };
	     break;

	   case 6:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform1[i] = sample[i+8] & 0x0fff;
	       Waveform2[i] = sample[i+8+numSamples] & 0x0fff;
	     };
	     break;

	   case 7:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform0[i] = sample[i+8] & 0x0fff;
	       Waveform1[i] = sample[i+8+numSamples] & 0x0fff;
	       Waveform2[i] = sample[i+8+2*numSamples] & 0x0fff;
	     };
	     break;

	   case 8:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform3[i] = sample[i+8] & 0x0fff;
	     };
	     break;

	   case 9:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform0[i] = sample[i+8] & 0x0fff;
	       Waveform3[i] = sample[i+8+numSamples] & 0x0fff;
	     };
	     break;

	   case 10:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform1[i] = sample[i+8] & 0x0fff;
	       Waveform3[i] = sample[i+8+numSamples] & 0x0fff;
	     };
	     break;

	   case 11:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform0[i] = sample[i+8] & 0x0fff;
	       Waveform1[i] = sample[i+8+numSamples] & 0x0fff;
	       Waveform3[i] = sample[i+8+2*numSamples] & 0x0fff;
	     };
	     break;

	   case 12:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform2[i] = sample[i+8] & 0x0fff;
	       Waveform3[i] = sample[i+8+numSamples] & 0x0fff;
	     };
	     break;

	   case 13:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform0[i] = sample[i+8] & 0x0fff;
	       Waveform2[i] = sample[i+8+numSamples] & 0x0fff;
	       Waveform3[i] = sample[i+8+2*numSamples] & 0x0fff;
	     };
	     break;

	   case 14:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform1[i] = sample[i+8] & 0x0fff;
	       Waveform2[i] = sample[i+8+numSamples] & 0x0fff;
	       Waveform3[i] = sample[i+8+2*numSamples] & 0x0fff;
	     };
	     break;

	   case 15:
	     for(UInt_t i = 0 ; i<numSamples;i++){
	       Waveform0[i] = sample[i+8] & 0x0fff;
	       Waveform1[i] = sample[i+8+numSamples] & 0x0fff;
	       Waveform2[i] = sample[i+8+2*numSamples] & 0x0fff;
	       Waveform3[i] = sample[i+8+3*numSamples] & 0x0fff;
	     };
	     break;


	 }//end of switch(chanMask)

}

