// ORCaen965qdcTreeWriter.cc

#include "ORCaen965qdcTreeWriter.hh"
#include "ORLogger.hh"
#include <sstream> 
#include "ORRunContext.hh"
#include "ORDictionary.hh"

using namespace std;

ORCaen965qdcTreeWriter::ORCaen965qdcTreeWriter(string treeName) :
ORVTreeWriter(new ORCaen965qdcDecoder, treeName)
{
	fEventDecoder	    = dynamic_cast<ORCaen965qdcDecoder*>(fDataDecoder);
	fQuestionableEvent = 0;
	fSize = 0;
	fQDCVal[0]           = 0;  
	fChannel[0]          = 0; 
	fRangeBit[0]         = 0;
	fIsUnderThreshold[0] = 0;
	fIsOverflow[0]       = 0;
	fIsValidData[0]      = 0;
	SetDoNotAutoFillTree();
}

ORCaen965qdcTreeWriter::~ORCaen965qdcTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORCaen965qdcTreeWriter::InitializeBranches()
{
	fTree->Branch("fQuestionableEvent",	&fQuestionableEvent,	"fQuestionableEvent/i");
	fTree->Branch("fSize",				&fSize,					"fSize/i");
	fTree->Branch("fQDCVal",            fQDCVal,	        	"fQDCVal[fSize]/i");
	fTree->Branch("fChannel",           fChannel,           	"fChannel[fSize]/i");
	fTree->Branch("fRangeBit",          fRangeBit,          	"fRangeBit[fSize]/i");
	fTree->Branch("fIsUnderThreshold",  fIsUnderThreshold,		"fIsUnderThreshold[fSize]/i");
	fTree->Branch("fIsOverflow",        fIsOverflow,        	"fIsOverflow[fSize]/i");
	fTree->Branch("fIsValidData",       fIsValidData,	    	"fIsValidData[fSize]/i");
	return kSuccess;
}

ORDataProcessor::EReturnCode ORCaen965qdcTreeWriter::ProcessMyDataRecord(UInt_t* record)
{ 
	fEventDecoder->GetNRows(record);
//What are referred to as "Questionable events" are the events where the number of rows in the 
// caen record does not equal the number of enabled channels.  From past runs they are about 2%
// of events.
	if(fEventDecoder->NChannelsInBlock() != fEventDecoder->GetNRows(record)){
		fQuestionableEvent = 1;
		}
  	if(fEventDecoder->NChannelsInBlock() == fEventDecoder->GetNRows(record)){
		fQuestionableEvent = 0;
		}
	fSize				   = fEventDecoder->GetNRows(record);    
    UInt_t var = 0;
	for(size_t i=0;i<fEventDecoder->GetNRows(record); i++) {
		fChannel[var]          = fEventDecoder->IthChannelOf(record, i);
		fQDCVal[var]           = fEventDecoder->IthValueOf(record, i);
		fRangeBit[var]         = fEventDecoder->IthRangeBitOf(record, i);
		fIsUnderThreshold[var] = fEventDecoder->IthValueIsUnderThreshold(record, i);
		fIsOverflow[var]       = fEventDecoder->IthValueIsOverflow(record, i);
		fIsValidData[var]      = fEventDecoder->IthValueIsValid(record, i);
		var++;															
// 		if((fEventDecoder->NChannelsInBlock() != fEventDecoder->GetNRows(record))){     
// 		  std::cout << "num Channels: " << fEventDecoder->NChannelsInBlock() << std::endl;
// 		  std::cout << "num Rows: " << fEventDecoder->GetNRows(record) << std::endl;
// 		  Dump(record, i);
// 	} 
	}	
  fTree->Fill();
return kSuccess;
}

void ORCaen965qdcTreeWriter::Dump(UInt_t* record, size_t iRow)
{
  std::cout << "---------------------------------------------" << std::endl;
  std::cout << "---------------------------------------------" << std::endl;
  std::cout << "Record:  " << record << ",  iRow:  " << iRow << std::endl;
  std::cout << "\tfQuestionableEvent :  " << fQuestionableEvent <<std::endl;
  std::cout << "\tfSize				 :  " << fSize << std::endl;
  std::cout << "\tfChannel           :  " << fChannel[iRow] << std::endl;
  std::cout << "\tfQDCVal            :  " << fQDCVal[iRow] << std::endl;
  std::cout << "\tfIsUnderThreshold  :  " << fIsUnderThreshold[iRow] << std::endl;
  std::cout << "\tfIsOverflow        :  " << fIsOverflow[iRow] << std::endl;
  std::cout << "\tfIsValidData       :  " << fIsValidData[iRow] << std::endl;
  std::cout << "---------------------------------------------" << std::endl;
}