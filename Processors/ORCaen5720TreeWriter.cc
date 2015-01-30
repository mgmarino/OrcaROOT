// ORCaen5720TreeWriter.cc

#include "ORCaen5720TreeWriter.hh"

#include "ORLogger.hh"

using namespace std;



ORCaen5720TreeWriter::ORCaen5720TreeWriter(string treeName) :
ORVTreeWriter(new ORCaen5720Decoder, treeName)
{
  fCaen5720Decoder = dynamic_cast<ORCaen5720Decoder*>(fDataDecoder); // just renaming
  Clear();
}

ORCaen5720TreeWriter::~ORCaen5720TreeWriter()
{
  delete fCaen5720Decoder;
}

ORDataProcessor::EReturnCode ORCaen5720TreeWriter::InitializeBranches()
{
  fTree->Branch("numSamples", &fnumSamples, "numSamples/i");
  fTree->Branch("clock", &fClock, "clock/i");
  fTree->Branch("eventcount", &fEventCount, "eventcount/i");
  fTree->Branch("channelmask", &fChannelMask, "channelmask/i");
  fTree->Branch("waveform", fwaveform, "fwaveform[numSamples]/i");
  fTree->Branch("waveform0", fwaveform0, "fwaveform0[numSamples]/i");
  fTree->Branch("waveform1", fwaveform1, "fwaveform1[numSamples]/i");
  fTree->Branch("waveform2", fwaveform2, "fwaveform2[numSamples]/i");
  fTree->Branch("waveform3", fwaveform3, "fwaveform3[numSamples]/i");
 
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCaen5720TreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  //check severity to improve speed
  if(ORLogger::GetSeverity() >= ORLogger::kDebug) { 
    ORLog(kDebug) << "ProcessMyDataRecord(): got one: "
                  << "CAEN Digitizer  " << endl;
  }

  bool dataispacked = fCaen5720Decoder->Packed(record);
  if(dataispacked){cout << "ERROR: Cannot process pack2.5 data. \n You must add this functionality to the ORDT5720Decoder" << endl; }
    
    fnumSamples =fCaen5720Decoder->TraceLength(record);
    fEventCount = fCaen5720Decoder->EventCount(record);
    fChannelMask = fCaen5720Decoder->ChannelMask(record);
    fClock = fCaen5720Decoder->Clock(record);
    fCaen5720Decoder->CopyTrace(record,fwaveform, fnumSamples); 
    fCaen5720Decoder->CopyTraces(record,fwaveform0,fwaveform1,fwaveform2,fwaveform3,fnumSamples);


  return kSuccess;
}

