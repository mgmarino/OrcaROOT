// ORShaperShaperTreeWriter.cc

#include "ORShaperShaperTreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORShaperShaperTreeWriter::ORShaperShaperTreeWriter(string treeName) :
ORVTreeWriter(new ORShaperShaperDecoder, treeName)
{
  fShaperShaperDecoder = dynamic_cast<ORShaperShaperDecoder*>(fDataDecoder); // just renaming
  fCrate = 0;
  fCard = 0;
  fChannel = 0;
  fADC = 0;
}

ORShaperShaperTreeWriter::~ORShaperShaperTreeWriter()
{
  delete fShaperShaperDecoder;
}

ORDataProcessor::EReturnCode ORShaperShaperTreeWriter::InitializeBranches()
{
  fTree->Branch("crate", &fCrate, "crate/I");
  fTree->Branch("card", &fCard, "card/I");
  fTree->Branch("channel", &fChannel, "channel/I");
  fTree->Branch("adc", &fADC, "adc/I");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORShaperShaperTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // check severity to improve speed
  if(ORLogger::GetSeverity() >= ORLogger::kDebug) { 
    ORLog(kDebug) << "ProcessMyDataRecord(): got one: "
                  << "cr-ca-ch-adc = " 
		  << fShaperShaperDecoder->CrateOf(record) << "-"
                  << fShaperShaperDecoder->CardOf(record) << "-" 
		  << fShaperShaperDecoder->ChannelOf(record) << "-" 
                  << fShaperShaperDecoder->ADCValueOf(record) << endl;
  }

  fCrate = fShaperShaperDecoder->CrateOf(record);
  fCard = fShaperShaperDecoder->CardOf(record);
  fChannel = fShaperShaperDecoder->ChannelOf(record);
  fADC = fShaperShaperDecoder->ADCValueOf(record);

  return kSuccess;
}

