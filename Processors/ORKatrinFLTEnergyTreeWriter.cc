// ORKatrinFLTEnergyTreeWriter.cc

#include "ORKatrinFLTEnergyTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORKatrinFLTEnergyTreeWriter::ORKatrinFLTEnergyTreeWriter(string treeName) :
ORVTreeWriter(new ORKatrinFLTEnergyDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORKatrinFLTEnergyDecoder*>(fDataDecoder);
  Clear();
}

ORKatrinFLTEnergyTreeWriter::~ORKatrinFLTEnergyTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORKatrinFLTEnergyTreeWriter::InitializeBranches()
{
  fTree->Branch("eventSec", &fSec, "eventSec/i");
  fTree->Branch("eventSubSec", &fSubSec, "eventSubSec/i");
  fTree->Branch("resetSec", &fResetSec, "resetSec/i");
  fTree->Branch("resetSubSec", &fResetSubSec, "resetSubSec/i");
  fTree->Branch("eventID", &fEventID, "eventID/i");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("energy", &fEnergy, "energy/i");
  fTree->Branch("channelMap", &fChannelMap, "channelMap/i");
  fTree->Branch("pageNumber", &fPageNumber, "pageNumber/s");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORKatrinFLTEnergyTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
      // check severity to improve speed:
  fSec = fEventDecoder->SecondsOf(record);
  fSubSec = fEventDecoder->SubSecondsOf(record);
  fResetSec = fEventDecoder->ResetSecondsOf(record);
  fResetSubSec = fEventDecoder->ResetSubSecondsOf(record);
  fEnergy = fEventDecoder->EnergyOf(record);
  fCrate = fEventDecoder->CrateOf(record);
  fCard = fEventDecoder->CardOf(record);
  fChannel = fEventDecoder->ChannelOf(record);
  fChannelMap = fEventDecoder->ChannelMapOf(record);
  fPageNumber = fEventDecoder->PageNumberOf(record);
  fEventID = fEventDecoder->EventIDOf(record);
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "sec-subsec-ID-crate-card-channel-energy = "
      << fSec << "-" << fSubSec << "-" << fCrate << "-"
      << fEventID << "-"
      << fCard << "-" << fChannel << "-" << fEnergy << endl;
  }
  return kSuccess;
}

/**  End decoding the current file. Writes the tree to the output file.
  *  This is almost an exact copy of the original ORVTreeWriter::EndRun()
  *  except that empty trees are not written to the root file if the flag 
  *  saveOnlyNonemptyTrees is set to @e true.
  *
  *  
  */   // -tb- 2008-02-19
ORDataProcessor::EReturnCode ORKatrinFLTEnergyTreeWriter::EndRun()
{
  // Write the tree only if this processor is meant to write it.
  // fUniqueTree implies that the tree was manually filled; it still should
  // be written automatically. A manual write can be performed by
  // overloading this function.
  if (fThisProcessorAutoFillsTree || fUniqueTree) {
    if (fFillMethod == kFillBeforeProcessDataRecord && fLastProcessedRecordRetCode == kSuccess) fTree->Fill();

    if (fTree->GetEntries() == 0) {
      ORLog(kWarning) << "EndRun(): no entries in fTree " << fTree->GetName() << endl;
    }

    if (fThisProcessorAutoFillsTree) {
      if (fTree->GetEntries() != fFillCount/fFillPeriod) {
        ORLog(kWarning) << fFillCount << ' ' << fFillPeriod << endl;
        ORLog(kWarning) << "EndRun(): expected tree to have "
                        << fFillCount/fFillPeriod << " entries, but found "
		        << fTree->GetEntries() << ": did more than one "
		        << "processor try to fill this tree? " << endl;
      }
    }

    if(saveOnlyNonemptyTrees){// -tb- 2008-02-19
      if (fTree->GetEntries() != 0) fTree->Write(fTree->GetName(), TObject::kOverwrite);
      else ORLog(kWarning) << "EndRun(): did not write empty fTree " << fTree->GetName() << endl;

    }else{
       fTree->Write(fTree->GetName(), TObject::kOverwrite);
    }
  }
  return kSuccess;
}

