// ORL2551ScalersTreeWriter.cc

#include "ORL2551ScalersTreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORL2551ScalersTreeWriter::ORL2551ScalersTreeWriter(string treeName) :
ORVTreeWriter(new ORL2551ScalersDecoder, treeName)
{
  fScalersDecoder = dynamic_cast<ORL2551ScalersDecoder*>(fDataDecoder);
  fCrate = 0;
  fCard = 0;
  fChannel = 0;
  fScaler = 0;
  SetDoNotAutoFillTree();
}

ORL2551ScalersTreeWriter::~ORL2551ScalersTreeWriter()
{
  delete fScalersDecoder;
}

ORDataProcessor::EReturnCode ORL2551ScalersTreeWriter::InitializeBranches()
{
  fTree->Branch("crate", &fCrate, "crate/I");
  fTree->Branch("card", &fCard, "card/I");
  fTree->Branch("channel", &fChannel, "channel/I");
  fTree->Branch("scaler", &fScaler, "scaler/I");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORL2551ScalersTreeWriter::ProcessMyDataRecord(UInt_t* record)
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

