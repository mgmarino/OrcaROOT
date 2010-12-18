// ORSIS3820TreeWriter.cc

#include "ORSIS3820TreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORSIS3820TreeWriter::ORSIS3820TreeWriter(string treeName) :
ORVTreeWriter(new ORSIS3820Decoder, treeName)
{
  fScalersDecoder = dynamic_cast<ORSIS3820Decoder*>(fDataDecoder);
  fCrate = 0;
  fCard = 0;
  fChannel = 0;
  fScaler = 0;
  SetDoNotAutoFillTree();
}

ORSIS3820TreeWriter::~ORSIS3820TreeWriter()
{
  delete fScalersDecoder;
}

ORDataProcessor::EReturnCode ORSIS3820TreeWriter::InitializeBranches()
{
  fTree->Branch("crate", &fCrate, "crate/I");
  fTree->Branch("card", &fCard, "card/I");
  fTree->Branch("channel", &fChannel, "channel/I");
  fTree->Branch("scaler", &fScaler, "scaler/I");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORSIS3820TreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // check severity to improve speed
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) { 
    ORLog(kDebug) << "ProcessMyDataRecord(): got one: "
                  << "crate-card-nscalers = " 
		  << fScalersDecoder->CrateOf(record) << "-"
		  << fScalersDecoder->CardOf(record) << "-" 
                  << fScalersDecoder->NScalersOf(record) << endl;
  }

  fCrate = fScalersDecoder->CrateOf(record);
  fCard = fScalersDecoder->CardOf(record);
  for(size_t i=0; i<fScalersDecoder->NScalersOf(record); i++) {
    fChannel = fScalersDecoder->IthChannelOf(record, i);
    fScaler = fScalersDecoder->IthScalerOf(record, i);
    fTree->Fill();
  }

  return kSuccess;
}

