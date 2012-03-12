// ORVXMTreeWriter.cc

#include "ORVXMTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORVXMTreeWriter::ORVXMTreeWriter(string treeName) :
ORVTreeWriter(new ORVXMDecoder, treeName)
{
  fVXMDecoder = dynamic_cast<ORVXMDecoder*>(fDataDecoder); 
  Clear();
  SetDoNotAutoFillTree();
}

ORVXMTreeWriter::~ORVXMTreeWriter()
{
  delete fVXMDecoder;
}

ORDataProcessor::EReturnCode ORVXMTreeWriter::InitializeBranches()
{
  fTree->Branch("fNumberOfMotors", &fNumberOfMotors, "fNumberOfMotors/i");
  fTree->Branch("fTime", &fTime, "fTime/L");
  fTree->Branch("fMotorID", &fMotorID, "fMotorID/s");
  fTree->Branch("fRawPosition", &fRawPosition, "fRawPosition/F"); 
  fTree->Branch("fPositon_in_mm", &fPositon_in_mm, "fPositon_in_mm/F"); 
  fTree->Branch("fConversion", &fConversion, "fConversion/F");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORVXMTreeWriter::ProcessMyDataRecord(UInt_t* record)
{

	if(ORLogger::GetSeverity() >= ORLogger::kDebug) { fVXMDecoder->Dump(record); }
	fNumberOfMotors = fVXMDecoder->GetNumberOfMotors();
	fTime           = fVXMDecoder->GetTime(record);
	fMotorID        = fVXMDecoder->GetMotorID(record);
	fRawPosition    = fVXMDecoder->GetRawPosition(record);       //steps
	fPositon_in_mm  = fVXMDecoder->GetConvertedPosition(record); // mm
	fConversion     = fVXMDecoder->GetConversion(record);        //steps/mm
	
	fTree->Fill();
  
  return kSuccess;
}

