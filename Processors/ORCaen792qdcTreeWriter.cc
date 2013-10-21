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
  fChannel.assign(fEventDecoder->GetNChannels(), 0);
  fQDCVal.assign(fEventDecoder->GetNChannels(), 0);
  fIsUnderThreshold.assign(fEventDecoder->GetNChannels(), 0);
  fIsOverflow.assign(fEventDecoder->GetNChannels(), 0);
  fIsValidData.assign(fEventDecoder->GetNChannels(), 0);
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
}

ORDataProcessor::EReturnCode ORCaen792qdcTreeWriter::InitializeBranches()
{
  fTree->Branch("crate",         &fCrate,             "cr/i");
  fTree->Branch("card",          &fCard,              "ca/i");
  fTree->Branch("ch",            &fChannel);
  fTree->Branch("qdc",           &fQDCVal);
  fTree->Branch("isUnderThresh", &fIsUnderThreshold);
  fTree->Branch("isOverflow",    &fIsOverflow);
  fTree->Branch("isValid",       &fIsValidData);
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCaen792qdcTreeWriter::ProcessMyDataRecord(UInt_t* record)
{ 
  fCrate = fEventDecoder->CrateOf(record);
  fCard = fEventDecoder->CardOf(record);
  size_t n = fEventDecoder->GetNRows(record);
  fChannel.resize(n);
  fQDCVal.resize(n);
  fIsUnderThreshold.resize(n);
  fIsOverflow.resize(n);
  fIsValidData.resize(n);
  for(size_t i=0; i<n; i++) {
    fChannel[i]          = fEventDecoder->IthChannelOf(record, i);
    fQDCVal[i]           = fEventDecoder->IthValueOf(record, i);
    fIsUnderThreshold[i] = fEventDecoder->IthValueIsUnderThreshold(record, i);
    fIsOverflow[i]       = fEventDecoder->IthValueIsOverflow(record, i);
    fIsValidData[i]      = fEventDecoder->IthValueIsValid(record, i);
  }	
  fTree->Fill();
  return kSuccess;
}

void ORCaen792qdcTreeWriter::Dump(UInt_t* record, size_t iRow)
{
  cout << "---------------------------------------------" << endl;
  cout << "---------------------------------------------" << endl;
  cout << "Record:  " << record << ",  iRow:  " << iRow   << endl;
  cout << "\tfCrate             :  " << fCrate            << endl;
  cout << "\tfCard              :  " << fCard             << endl;
  cout << "\tSize               :  " << fChannel.size()   << endl;
  cout << "\tfChannel           :  " << fChannel[iRow]    << endl;
  cout << "\tfQDCVal            :  " << fQDCVal[iRow]     << endl;
  cout << "\tfIsUnderThreshold  :  " << fIsUnderThreshold[iRow] << endl;
  cout << "\tfIsOverflow        :  " << fIsOverflow[iRow] << endl;
  cout << "\tfIsValidData       :  " << fIsValidData[iRow] << endl;
  cout << "---------------------------------------------" << endl;
}
