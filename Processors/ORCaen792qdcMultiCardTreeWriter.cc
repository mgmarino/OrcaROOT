// ORCaen792qdcMultiCardTreeWriter.cc

#include "ORCaen792qdcMultiCardTreeWriter.hh"
#include "ORCaen792NqdcDecoder.hh"
#include "ORLogger.hh"

using namespace std;

ORCaen792qdcMultiCardTreeWriter::ORCaen792qdcMultiCardTreeWriter(ORCaen792qdcMultiCardTreeWriter::E792Model model, std::string treeName) :
ORVTreeWriter(NULL, treeName)
{
  if(model == k792A) fDataDecoder = new ORCaen792qdcDecoder;
  else if(model == k792N) fDataDecoder = new ORCaen792NqdcDecoder;
  else {
    ORLog(kError) << "invalid model " << model << ", usign 792A" << endl;
    fDataDecoder = new ORCaen792qdcDecoder;
  }
  fEventDecoder = dynamic_cast<ORCaen792qdcDecoder*>(fDataDecoder);
  fEventCount = 0;
  fNValues = 0;
  SetDoNotAutoFillTree();
}

ORCaen792qdcMultiCardTreeWriter::~ORCaen792qdcMultiCardTreeWriter()
{
  delete fEventDecoder;
  fDataDecoder = NULL;
}

void ORCaen792qdcMultiCardTreeWriter::Clear() 
{ 
  fNValues = 0;
  size_t n = fEventDecoder->GetNChannels()*fCard.size();
  fQDCVal.assign(n, 0);
  fIsUnderThreshold.assign(n, 0);
  fIsOverflow.assign(n, 0);
}

ORDataProcessor::EReturnCode ORCaen792qdcMultiCardTreeWriter::InitializeBranches()
{
  fTree->Branch("crate",         &fCrate);
  fTree->Branch("card",          &fCard);
  fTree->Branch("eventCount",    &fEventCount,        "nE/i");
  fTree->Branch("nValues",       &fNValues,           "nV/i");
  fTree->Branch("qdc",           &fQDCVal);
  fTree->Branch("isUnderThresh", &fIsUnderThreshold);
  fTree->Branch("isOverflow",    &fIsOverflow);
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCaen792qdcMultiCardTreeWriter::BeginRun()
{
  fEventCount = 0;
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCaen792qdcMultiCardTreeWriter::ProcessMyDataRecord(UInt_t* record)
{ 
  cout << "E:N " << fEventCount << ' ' << fNValues << endl;
  if(fEventDecoder->EventCountOf(record) != fEventCount && fNValues > 0) {
    if(fNValues < fEventDecoder->GetNChannels()*fCard.size()) {
      ORLog(kWarning) << "Bad fNValues at event count " << fEventCount 
                      << ": skipped card?" << endl;
    }
    fTree->Fill();
    Clear();
  }
  if(fEventDecoder->EventCountOf(record) > fEventCount + 1) {
    if(fEventCount == 0) {
      ORLog(kWarning) << "Bad event count " 
                      << fEventDecoder->EventCountOf(record) 
                      << " at start of run, skipping..." << endl;
      return kSuccess;
    }
    else {
      ORLog(kWarning) << "Data skipped from event " << fEventCount 
                      << " to " << fEventDecoder->EventCountOf(record) << endl;
    }
  }
  fEventCount = fEventDecoder->EventCountOf(record);

  // Going to look for (crate,card) at location iCard, since card readout should
  // occur in-order, every time.
  UInt_t crate = fEventDecoder->CrateOf(record);
  UInt_t card = fEventDecoder->CardOf(record);
  size_t iCard = fNValues/fEventDecoder->GetNChannels();

  cout << "i:N:s " << iCard << ' ' << fNValues << ' ' << fCard.size() << endl;

  // If iCard is too big, then this is a new card. Add it to the list.
  if(iCard >= fCard.size()) {
    ORLog(kRoutine) << "adding (crate,card) = (" << crate << "," << card 
                    << ") at index " << fCard.size() << endl;
    fCrate.push_back(crate);
    fCard.push_back(card);
    size_t n = fEventDecoder->GetNChannels() + fQDCVal.size();
    fQDCVal.resize(n, 0);
    fIsUnderThreshold.resize(n, 0);
    fIsOverflow.resize(n, 0);
  }

  // While (crate,card) is not found, report that cards were skipped
  while(fCard[iCard] != card || fCrate[iCard] != crate) {
    ORLog(kWarning) << "Data for (crate,card) = (" << crate << "," << card 
                    << ") came in before that at (" << fCrate[iCard] << "," 
                    << fCard[iCard] << "), skipping it..." << endl;
    fNValues += fEventDecoder->GetNChannels();
    iCard++;
    if(iCard >= fCard.size()) {
      ORLog(kError) << "Data for (crate,card) = (" << crate << "," << card 
                    << ") came in before all known cards?!? Aborting... " << endl;
      return kAlarm;
    }
  }

  // Check that we are getting the right number of values out
  size_t nVals = fEventDecoder->NValuesOf(record);
  if(fEventDecoder->NValuesOf(record) != fEventDecoder->GetNChannels()) {
    ORLog(kWarning) << "(crate,card) = (" << crate << "," << card 
                    << ") had " << nVals << " vals when " << fEventDecoder->GetNChannels()
                    << " were expected." << endl;
    if(nVals > fEventDecoder->GetNChannels()) nVals = fEventDecoder->GetNChannels();
  }

  // Okay. Now pull out the actual QDC values
  for(size_t i=0; i<nVals; i++) {
    size_t iCh = fEventDecoder->IthChannelOf(record, i);
    if(iCh >= fEventDecoder->GetNChannels()) {
      ORLog(kError) << "invalid channel " << iCh << ", hex dump and skip" << endl;
      fEventDecoder->DumpHex(record);
      continue;
    }
    if(fNValues+i >= fQDCVal.size()) {
      ORLog(kError) << "index out-of-range" << endl;
      return kFailure;
    }
    fQDCVal[fNValues+iCh]           = fEventDecoder->IthValueOf(record, i);
    fIsUnderThreshold[fNValues+iCh] = fEventDecoder->IthValueIsUnderThreshold(record, i);
    fIsOverflow[fNValues+iCh]       = fEventDecoder->IthValueIsOverflow(record, i);
  }	
  fNValues += fEventDecoder->GetNChannels();
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCaen792qdcMultiCardTreeWriter::EndRun()
{
  fTree->Fill();
  return kSuccess;
}
