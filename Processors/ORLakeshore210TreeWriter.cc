// ORLakeshore210TreeWriter.cc

#include "ORLakeshore210TreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORLakeshore210TreeWriter::ORLakeshore210TreeWriter(string treeName) :
ORVTreeWriter(new ORLakeshore210Decoder, treeName)
{
  fLakeshore210Decoder = dynamic_cast<ORLakeshore210Decoder*>(fDataDecoder); 
  Clear();
  SetDoNotAutoFillTree();
}

ORLakeshore210TreeWriter::~ORLakeshore210TreeWriter()
{
  delete fLakeshore210Decoder;
}

ORDataProcessor::EReturnCode ORLakeshore210TreeWriter::InitializeBranches()
{
  fTree->Branch("isCelsius", &fIsCelsius, "isCelsius/O");
  fTree->Branch("temperature", &fTemperature, "temp/F");
  fTree->Branch("time", &fTime, "time/i");
  fTree->Branch("channel", &fChannel, "channel/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORLakeshore210TreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  fIsCelsius = (fLakeshore210Decoder->GetTemperatureUnits(record) 
    == ORLakeshore210Decoder::kCelsius);
  for (UInt_t i=0; i<fLakeshore210Decoder->GetNumberOfChannels();i++) {
    fTemperature = fLakeshore210Decoder->GetTempOfChannel(record, i);
    fTime = fLakeshore210Decoder->GetTimeOfChannel(record, i);
    fChannel = i;
    if(ORLogger::GetSeverity() >= ORLogger::kDebug) { 
      ORLog(kDebug) << "channel:temp:unit:time = "  
        << fChannel << ":" << fTemperature << ":" 
        << ((fIsCelsius) ? 'C' : 'K') 
        << ":" << fTime << endl;
    }
    fTree->Fill();
  }

  return kSuccess;
}

