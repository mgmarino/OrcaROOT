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
  fTree->Branch("fIsRunning", &fIsRunning, "fIsRunning/s");
  fTree->Branch("fX", &fX, "fX/D");
  fTree->Branch("fY", &fY, "fY/D");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORVXMTreeWriter::ProcessMyDataRecord(UInt_t* record)
{

	if(ORLogger::GetSeverity() >= ORLogger::kDebug) { fVXMDecoder->Dump(record); }
  
	fNumberOfMotors = fVXMDecoder->GetNumberOfMotors();
	fTime           = fVXMDecoder->GetTime(record);
	fIsRunning      = fVXMDecoder->GetIsRunning(record);
	fX              = fVXMDecoder->GetXPosition(record);
	fY              = fVXMDecoder->GetYPosition(record);
	
	fTree->Fill();
  
  return kSuccess;
}

