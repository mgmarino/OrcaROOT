// ORTrig4ChanTreeWriter.cc

#include "ORTrig4ChanTreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORTrig4ChanTreeWriter::ORTrig4ChanTreeWriter(string treeName) :
ORVTreeWriter(new ORTrig4ChanDecoder, treeName)
{
  fTrig4ChanDecoder = dynamic_cast<ORTrig4ChanDecoder*>(fDataDecoder); // just renaming
  ftrig = 0;
  fUpClock = 0;
  fLowClock = 0;
   fClock = 0;
}

ORTrig4ChanTreeWriter::~ORTrig4ChanTreeWriter()
{
  delete fTrig4ChanDecoder;
}

ORDataProcessor::EReturnCode ORTrig4ChanTreeWriter::InitializeBranches()
{
  fTree->Branch("trigger", &ftrig, "trigger/i");
  fTree->Branch("UpClock", &fUpClock, "Upclock/i");
  fTree->Branch("LowClock", &fLowClock, "LowClock/i");
   fTree->Branch("Clock", &fClock, "Clock/l");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORTrig4ChanTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // check severity to improve speed
  if(ORLogger::GetSeverity() >= ORLogger::kDebug) { 
    ORLog(kDebug) << "ProcessMyDataRecord(): got one: "
                  << "trig-up-low = " 
                  << fTrig4ChanDecoder->trigOf(record) << "-" 
		  << fTrig4ChanDecoder->UpClockOf(record) << "-" 
     << fTrig4ChanDecoder->LowClockOf(record) << "-" 
                  << fTrig4ChanDecoder->ClockOf(record) << endl;
  }

  ftrig = fTrig4ChanDecoder->trigOf(record);
  fUpClock  = fTrig4ChanDecoder->UpClockOf(record);
  fLowClock = fTrig4ChanDecoder->LowClockOf(record);
   fClock = fTrig4ChanDecoder->ClockOf(record);

  return kSuccess;
}

