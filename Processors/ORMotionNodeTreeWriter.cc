// ORMotionNodeTreeWriter.cc

#include "ORMotionNodeTreeWriter.hh"
#include "ORLogger.hh"
#include <sstream> 
#include "ORRunContext.hh"
#include "ORDictionary.hh"

using namespace std;

ORMotionNodeTreeWriter::ORMotionNodeTreeWriter(string treeName) :
ORVTreeWriter(new ORMotionNodeDecoder, treeName)
{
	fEventDecoder	= dynamic_cast<ORMotionNodeDecoder*>(fDataDecoder);
	fDevice			= 0;
	fChannel		= 0;
	fWaveformLength = 0;
	SetDoNotAutoFillTree();
}

ORMotionNodeTreeWriter::~ORMotionNodeTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORMotionNodeTreeWriter::InitializeBranches()
{
	fTree->Branch("wLength", &fWaveformLength, "wLength/i");
	fTree->Branch("device",   &fDevice,			"device/s");
	fTree->Branch("channel",  &fChannel,		"channel/s");
	fTree->Branch("waveform", fWaveform,		"waveform[wLength]/i");
	return kSuccess;
}

ORDataProcessor::EReturnCode ORMotionNodeTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
	// the event decoder could run into a problem, but this might not
	// ruin the rest of the run.
	if(!fEventDecoder->SetDataRecord(record)) return kFailure;
	fDevice			= fEventDecoder->Device();
	fChannel		= fEventDecoder->GetChannelNum();
	fWaveformLength = fEventDecoder->GetWaveformLength();
	if (ORLogger::GetSeverity() >= ORLogger::kDebug) { 
		ORLog(kDebug) << "ProcessMyDataRecord(): "
			<< "device-channel-length- = "
			<< fDevice << "-"
			<< fChannel << "-"
			<< fWaveformLength << "-"
			<< endl;
	}

	if (fWaveformLength > kMaxwLength) {
		ORLog(kError) << "Waveform too long for kMaxwLength!" << endl;
		return kFailure;
	}
	fEventDecoder->CopyWaveformData(fWaveform, fWaveformLength);
	fTree->Fill();
	return kSuccess;
}

