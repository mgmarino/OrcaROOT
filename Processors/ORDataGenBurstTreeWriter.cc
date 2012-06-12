// ORDataGenBurstTreeWriter.cc

#include "ORDataGenBurstTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORDataGenBurstTreeWriter::ORDataGenBurstTreeWriter(string treeName) : 
ORVTreeWriter(new ORDataGenBurstDataDecoder, treeName)
{
	fBurstDecoder = dynamic_cast<ORDataGenBurstDataDecoder*>(fDataDecoder);
	fCrate = 0;
	fCard = 0;
	fChannel = 0;
	fValue = 0;
	fTime = 0;
	fLastTime = 0;
	fDiffTime = 0;
	SetDoNotAutoFillTree();
}

ORDataGenBurstTreeWriter::~ORDataGenBurstTreeWriter()
{
	delete fBurstDecoder;
}

ORDataGenBurstTreeWriter::EReturnCode ORDataGenBurstTreeWriter::InitializeBranches()
{
	fTree->Branch("crate",   &fCrate,   "crate/I");
	fTree->Branch("card",    &fCard,    "card/I");
	fTree->Branch("channel", &fChannel, "channel/I");
	fTree->Branch("value",   &fValue,   "value/I");
	fTree->Branch("time",    &fTime,    "time/I");
	fTree->Branch("diffTime",&fDiffTime, "diffTime/I");
	return kSuccess;
}

ORDataGenBurstTreeWriter::EReturnCode ORDataGenBurstTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
	// check severity to improve speed


	fCrate   = fBurstDecoder->CrateOf(record);
	fCard    = fBurstDecoder->CardOf(record);
	fChannel = fBurstDecoder->ChannelOf(record);
	fValue   = fBurstDecoder->ValueOf(record);
	fTime    = fBurstDecoder->TimeOf(record);

	
	if (ORLogger::GetSeverity() >= ORLogger::kDebug) { 
		ORLog(kDebug)	<< "ProcessMyDataRecord(): got one: "
		<< "crate-card-channel-value-time = " 
		<< fBurstDecoder->CrateOf(record) << "-"
		<< fBurstDecoder->CardOf(record) << "-" 
		<< fBurstDecoder->ChannelOf(record) << "-" 
		<< fBurstDecoder->ValueOf(record) << "-" 
		<< fBurstDecoder->TimeOf(record) << "-"
		<< fDiffTime << endl;
	}
	if(fLastTime == 0){
		fLastTime = fTime;
	}
	else {
		fDiffTime = fTime - fLastTime;
		fLastTime = fTime;
	}
	
	
	fTree->Fill();
  

  return kSuccess;
}

