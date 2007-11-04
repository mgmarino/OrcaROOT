// ORGretaWaveformTreeWriter.cc

#include "ORGretaWaveformTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORGretaWaveformTreeWriter::ORGretaWaveformTreeWriter(string treeName) :
ORVTreeWriter(new ORGretaDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORGretaDecoder*>(fDataDecoder);
  fLEDEventTime = 0.;
  fCFDEventTime = 0.;
  fCrate = 0;
  fCard = 0;
  fChannel = 0;
  fBoard = 0;
  fEnergy = 0;
  fWaveformLength = 0;
  fWaveform[0] = 0;  // this is on the stack, not the heap.
  SetDoNotAutoFillTree();
}

ORGretaWaveformTreeWriter::~ORGretaWaveformTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORGretaWaveformTreeWriter::InitializeBranches()
{
  fTree->Branch("wfLength", &fWaveformLength, "wfLength/i");
  fTree->Branch("LEDEventTime", &fLEDEventTime, "LEDEventTime/D");
  fTree->Branch("CFDEventTime", &fCFDEventTime, "CFDEventTime/D");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("board", &fBoard, "board/s");
  fTree->Branch("energy", &fEnergy, "energy/i");
  fTree->Branch("waveform", fWaveform, "waveform[wfLength]/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORGretaWaveformTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "LEDtime-crate-card-channel-energy = "
      << fEventDecoder->GetLEDExtTimeStamp() << "-"
      << fEventDecoder->CrateOf() << "-"
      << fEventDecoder->CardOf() << "-" 
      << fEventDecoder->GetChannelNum() << "-"
      << fEventDecoder->GetEnergy() << endl;
  }
  fLEDEventTime = fEventDecoder->GetLEDExtTimeStamp();
  fCFDEventTime = fEventDecoder->GetCFDTimeStamp();
  fCrate = fEventDecoder->CrateOf();
  fCard = fEventDecoder->CardOf();
  fBoard = fEventDecoder->GetBoardId();
  fChannel = fEventDecoder->GetChannelNum();
  fEnergy = fEventDecoder->GetEnergy();
  fWaveformLength = fEventDecoder->CopyWaveformData(fWaveform, kMaxWFLength);
  fTree->Fill();
  return kSuccess;
}

