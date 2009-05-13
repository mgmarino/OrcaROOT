// ORMCA927TreeWriter.cc

#include "ORMCA927TreeWriter.hh"
#include "ORLogger.hh"
#include <sstream> 
#include "ORRunContext.hh"
#include "ORDictionary.hh"

using namespace std;

ORMCA927TreeWriter::ORMCA927TreeWriter(string treeName) :
ORVTreeWriter(new ORMCA927Decoder, treeName)
{
	fEventDecoder	= dynamic_cast<ORMCA927Decoder*>(fDataDecoder);
	fDevice			= 0;
	fChannel		= 0;
	fLiveTime		= 0;
	fRealTime		= 0;
	fWaveformLength = 0;
	SetDoNotAutoFillTree();
}

ORMCA927TreeWriter::~ORMCA927TreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORMCA927TreeWriter::InitializeBranches()
{
	fTree->Branch("wfLength", &fWaveformLength, "wfLength/i");
	fTree->Branch("device", &fDevice,			"device/s");
	fTree->Branch("type", &fType,				"type/s");
	fTree->Branch("channel", &fChannel,			"channel/s");
	fTree->Branch("spectrum", fWaveform,		"spectrum[wfLength]/i");
	fTree->Branch("zdt", fZDT,					"zdt[wfLength]/i");
	fTree->Branch("liveTime", &fLiveTime,		"liveTime/D");
	fTree->Branch("realTime", &fRealTime,		"realTime/D");
	return kSuccess;
}

ORDataProcessor::EReturnCode ORMCA927TreeWriter::ProcessMyDataRecord(UInt_t* record)
{
	// the event decoder could run into a problem, but this might not
	// ruin the rest of the run.
	if(!fEventDecoder->SetDataRecord(record)) return kFailure;
	fDevice			= fEventDecoder->GetDevice();
	fChannel		= fEventDecoder->GetChannelNum();
	fType			= fEventDecoder->GetType();
	fLiveTime		= fEventDecoder->GetLiveTime();
	fRealTime		= fEventDecoder->GetRealTime();
	fWaveformLength = fEventDecoder->GetWaveformLen();
	if (ORLogger::GetSeverity() >= ORLogger::kDebug) { 
		ORLog(kDebug) << "ProcessMyDataRecord(): "
			<< "time-device-channel-type-liveTime-realTime-length- = "
			<< fDevice << "-"
			<< fChannel << "-"
			<< fType << "-"
			<< fLiveTime << "-"
			<< fRealTime << "-"
			<< fWaveformLength << "-"
			<< endl;
	}

	if (fWaveformLength > kMaxWFLength) {
		ORLog(kError) << "Waveform too long for kMaxWFLength!" << endl;
		return kFailure;
	}
	if(fType == 0)fEventDecoder->CopyWaveformData(fWaveform, fWaveformLength);
	else		  fEventDecoder->CopyWaveformData(fZDT, fWaveformLength);
	fTree->Fill();
	return kSuccess;
}

