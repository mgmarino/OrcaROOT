
// ORDGF4cLiveTimeWriter.cc

#include "ORDGF4cLiveTimeWriter.hh"
#include "ORLogger.hh"
#include "ORRunContext.hh"

using namespace std;

ORDGF4cLiveTimeWriter::ORDGF4cLiveTimeWriter(string treeName) :
ORVTreeWriter(new ORDGF4cLiveTimeDecoder, treeName)
{
  fLiveTimeDecoder = dynamic_cast<ORDGF4cLiveTimeDecoder*>(fDataDecoder);
  fCrate = 0;
  fCard = 0;
  fRealTime = 0;
  fRunTime = 0;
  fChannel = 0;
  fLiveTime = 0;
  fNumEvents = 0;
  SetDoNotAutoFillTree();
  fLength = 0;
}

ORDGF4cLiveTimeWriter::~ORDGF4cLiveTimeWriter()
{
  delete fLiveTimeDecoder;
}

ORDataProcessor::EReturnCode ORDGF4cLiveTimeWriter::InitializeBranches()
{
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("realTime", &fRealTime, "realTime/D");
  fTree->Branch("runTime", &fRunTime, "runTime/D");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("liveTime", &fLiveTime, "liveTime/D");
  fTree->Branch("numEvents", &fNumEvents, "numEvents/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORDGF4cLiveTimeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fLiveTimeDecoder->SetDataRecord(record)) return kFailure;
  FindRecordLength();

  fCrate = fLiveTimeDecoder->CrateOf();
  fCard = fLiveTimeDecoder->CardOf();
  if((fLength == 13)||(fLength==11))
  {
    fRealTime = fLiveTimeDecoder->GetOldRealTime();
    fRunTime = fLiveTimeDecoder->GetOldRunTime();
  }else{
    fRealTime = fLiveTimeDecoder->GetRealTime();
    fRunTime = fLiveTimeDecoder->GetRunTime();
  }
  ORLog(kDebug) << "crate-card-realTime-runTime" << endl;
  ORLog(kDebug) << fCrate << "-" << fCard << "-" << fRealTime << "-" << fRunTime << endl;
  ORLog(kDebug) << "channel-liveTime-numEvents" << endl; 
  for(Int_t i = 0; i < 4; i++)
  {
    fChannel = i;
    if((fLength == 13)||(fLength==11))
    {
      fLiveTime = fLiveTimeDecoder->GetOldChanLiveTime(i);
      fNumEvents = fLiveTimeDecoder->GetOldChanNumEvents(i);
    }else{
      fLiveTime = fLiveTimeDecoder->GetChanLiveTime(i);
      fNumEvents = fLiveTimeDecoder->GetChanNumEvents(i);
    }
    fTree->Fill();
    ORLog(kDebug) << fChannel << "-" << fLiveTime << "-" << fNumEvents << endl;
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORDGF4cLiveTimeWriter::FindRecordLength()
{
  const ORDictionary* dict = NULL;
  if (fRunContext) {
    dict = dynamic_cast<const ORDictionary*>
      (fRunContext->GetHeader()->LookUp("dataDescription:ORDGF4cModel:LiveTime"));
  }
  if(!dict) 
  {
    ORLog(kError) << "Header corrupt!" << endl;
    return kFailure;
  }
  const ORDictValueI* length = dynamic_cast<const ORDictValueI*>
    (dict->LookUp("length"));
  if (!length) return kFailure;
  fLength = length->GetI(); // returns real value.
  return kSuccess;
}
