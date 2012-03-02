// ORSIS3302GenericDecoder.cc

#include "ORSIS3302GenericDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"


//**************************************************************************************

ORSIS3302GenericDecoder::ORSIS3302GenericDecoder() 
{ 
	fDataRecord = NULL; 
}

bool ORSIS3302GenericDecoder::SetDataRecord(UInt_t* dataRecord) 
{
	fDataRecord = dataRecord;
	
	ORLog(kDebug) << "SetDataRecord(): Setting the data record..." << std::endl;
	if(!IsValid()) {
		ORLog(kDebug) << "SetDataRecord(): data record is not valid" << std::endl;
		fDataRecord = NULL;
		return false;
	}
	ORLog(kDebug) << "SetDataRecord(): Exiting" << std::endl;
	return true;
}



//Channel functions: ******************************************************************

//copies the waveform data to the array pointed to by
//waveform, which is of length len
size_t ORSIS3302GenericDecoder::CopyWaveformData(UShort_t* waveform, size_t len)
{
	size_t wflen = GetWaveformLen();
	if (wflen == 0) return 0;
	if ((len < wflen) || (len == 0)) {
		ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
		<< "; waveform data length is " << GetWaveformLen() << std::endl;
	}
	else len = wflen; 
	memcpy(waveform, GetWaveformDataPointer(), len*2);
	return len;
}

//copies the waveform data to the array pointed to by
//waveform, which is of length len
size_t ORSIS3302GenericDecoder::CopyWaveformDataDouble(double* waveform, size_t len)
{
	size_t wflen = GetWaveformLen();
	if (wflen == 0) return 0;
	if ((len < wflen) || (len == 0)) {
		ORLog(kWarning) << "CopyWaveformData(): destination array length is " << len 
		<< "; waveform data length is " << GetWaveformLen() << std::endl;
	}
	else len = wflen; 
	const UInt_t* waveformData = GetWaveformDataPointer();
	for(size_t i=0;i<len;i+=2) 
	{
		waveform[i] = (double) (waveformData[i/2] & 0xFFFF);  
		waveform[i+1] = (double) (waveformData[i/2] >> 16);
	}
	return len;
}


/* Card/Channel settings, parameters. */

//Error checking: **********************************************************************



bool ORSIS3302GenericDecoder::IsValid() 
{ 
	ORLog(kDebug) << "IsValid(): starting... " << std::endl;
	if(IsShort(fDataRecord)) { 
		ORLog(kError) << "Data file is short" << std::endl; 
		return false;
	} 
	return true;
}

//debugging: *************************************************************************


void ORSIS3302GenericDecoder::Dump(UInt_t* dataRecord) //debugging 
{
	size_t iChannel, iEvent;
	
	ORLog(kDebug) << std::endl << std::endl << "ORSIS3302GenericDecoder::Dump():" << std::endl ;
	iChannel = 0;
	iEvent = 0;
	if(!SetDataRecord(dataRecord)) return; 
	ORLog(kDebug) 
	<< "  Header functions: " << std::endl
	<< "    Crate = " << CrateOf() << "; card = " << CardOf() << std::endl
	<< std::endl
	<< "    The channel is " << GetEventChannel(0) << std::endl
	<< std::endl  
	<< "    The waveform data has " << GetWaveformLen() 
	<< " (16-bit) words" << std::endl;
	
}

UInt_t ORSIS3302GenericDecoder::GetEventWaveformPoint( size_t /*event*/, 
											   size_t waveformPoint )
{
	if ( waveformPoint % 2 == 0 ) {
		return GetWaveformDataPointer()[waveformPoint/2] & 0xFFFF; 
	} else {
		return ( GetWaveformDataPointer()[waveformPoint/2] >> 16); 
	} 
}

