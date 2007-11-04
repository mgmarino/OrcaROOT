// ORTek754DScopeDataTreeWriter.cc

#include "ORTek754DScopeDataTreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORTek754DScopeDataTreeWriter::ORTek754DScopeDataTreeWriter(string treeName) :
ORVTreeWriter(new ORTek754DScopeDataDecoder, treeName)
{
  fScopeDataDecoder = dynamic_cast<ORTek754DScopeDataDecoder*>(fDataDecoder);
  fOwnsDecoder = true;
  fGPIBAddress = 0;
  fChannel = 0;
  fNData = 0;
  for (int i=0; i<kMaxNData; i++) {
    fData[i] = 0;
    fI[i] = i;
  }
}

ORTek754DScopeDataTreeWriter::ORTek754DScopeDataTreeWriter(ORTek754DScopeDataDecoder* decoder, string treeName) :
ORVTreeWriter(decoder, treeName)
{
  fScopeDataDecoder = decoder;
  fOwnsDecoder = false;
  fGPIBAddress = 0;
  fChannel = 0;
  fNData = 0;
  for (int i=0; i<kMaxNData; i++) {
    fData[i] = 0;
    fI[i] = i;
  }
}


ORTek754DScopeDataTreeWriter::~ORTek754DScopeDataTreeWriter()
{
  if(fOwnsDecoder) delete fScopeDataDecoder;
}

ORDataProcessor::EReturnCode ORTek754DScopeDataTreeWriter::InitializeBranches()
{
  fTree->Branch("gpibadd", &fGPIBAddress, "gpibadd/I");
  fTree->Branch("channel", &fChannel, "channel/I");
  fTree->Branch("n", &fNData, "n/I");
  fTree->Branch("data", fData, "data[n]/I");
  fTree->Branch("i", fI, "i[n]/I");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORTek754DScopeDataTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // check severity to improve speed
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) { 
    ORLog(kDebug) << "ProcessMyDataRecord(): got one: "
                  << "gpibadd-ch-n = " 
		  << fScopeDataDecoder->GPIBAddressOf(record) << "-"
		  << fScopeDataDecoder->ChannelOf(record) << "-" 
                  << fScopeDataDecoder->NDataOf(record) << endl;
  }

  fGPIBAddress = fScopeDataDecoder->GPIBAddressOf(record);
  fChannel = fScopeDataDecoder->ChannelOf(record);
  fNData = fScopeDataDecoder->NDataOf(record);
  if (fNData > kMaxNData) {
    ORLog(kWarning) << "ProcessMyDataRecord(): fNData (=" << fNData
                    << ") > kMaxNData (=" << kMaxNData << "); "
		    << "suggest increasing kMaxNData..." << endl;
    fNData = kMaxNData;
  }
  for (int i=0; i<fNData; i++) {
    fData[i] = fScopeDataDecoder->IthDataOf(record, i);
    if(fData[i] > 128 || fData[i] < -128) {
      ORLog(kWarning) << "ProcessMyDataRecord(): data point " << i
                      << "was out-of-range: = " << fData[i] << endl;
    }
  }

  return kSuccess;
}

