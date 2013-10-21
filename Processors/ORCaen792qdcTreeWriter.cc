// ORCaen792qdcTreeWriter.cc

#include "ORCaen792qdcTreeWriter.hh"
#include "ORCaen792NqdcDecoder.hh"
#include "ORLogger.hh"

using namespace std;

ORCaen792qdcTreeWriter::ORCaen792qdcTreeWriter(ORCaen792qdcTreeWriter::E792Model model, std::string treeName) :
ORVTreeWriter(NULL, treeName)
{
  if(model == k792A) fDataDecoder = new ORCaen792qdcDecoder;
  else if(model == k792N) fDataDecoder = new ORCaen792NqdcDecoder;
  else {
    ORLog(kError) << "invalid model " << model << ", usign 792A" << endl;
    fDataDecoder = new ORCaen792qdcDecoder;
  }
  fEventDecoder = dynamic_cast<ORCaen792qdcDecoder*>(fDataDecoder);
  fCrate = 0;
  fCard = 0;
  SetDoNotAutoFillTree();
}

ORCaen792qdcTreeWriter::~ORCaen792qdcTreeWriter()
{
  delete fEventDecoder;
  fDataDecoder = NULL;
}

void ORCaen792qdcTreeWriter::Clear() 
{ 
  fCrate = 0;
  fCard = 0;
  size_t n = fEventDecoder->GetNChannels();
  fQDCVal.assign(n, 0);
  fIsUnderThreshold.assign(n, 0);
  fIsOverflow.assign(n, 0);
  fIsValidData.assign(n, 0);
}

ORDataProcessor::EReturnCode ORCaen792qdcTreeWriter::InitializeBranches()
{
  fTree->Branch("crate",         &fCrate,             "cr/i");
  fTree->Branch("card",          &fCard,              "ca/i");
  fTree->Branch("qdc",           &fQDCVal);
  fTree->Branch("isUnderThresh", &fIsUnderThreshold);
  fTree->Branch("isOverflow",    &fIsOverflow);
  fTree->Branch("isValid",       &fIsValidData);
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCaen792qdcTreeWriter::ProcessMyDataRecord(UInt_t* record)
{ 
  Clear();
  fCrate = fEventDecoder->CrateOf(record);
  fCard = fEventDecoder->CardOf(record);
  for(size_t i=0; i<fEventDecoder->GetNRows(record); i++) {
    size_t iCh = fEventDecoder->IthChannelOf(record, i);
    if(iCh >= fEventDecoder->GetNChannels()) {
      if(fEventDecoder->IthValueIsValid(record, i)) { // okay to silently skip invalid records
        ORLog(kError) << "invalid channel " << iCh << ", skipping" << endl;
      }
      continue;
    }
    fQDCVal[iCh]           = fEventDecoder->IthValueOf(record, i);
    fIsUnderThreshold[iCh] = fEventDecoder->IthValueIsUnderThreshold(record, i);
    fIsOverflow[iCh]       = fEventDecoder->IthValueIsOverflow(record, i);
    fIsValidData[iCh]      = fEventDecoder->IthValueIsValid(record, i);
  }	
  fTree->Fill();
  return kSuccess;
}

