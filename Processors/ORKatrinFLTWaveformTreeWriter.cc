// ORKatrinFLTWaveformTreeWriter.cc

#include "ORKatrinFLTWaveformTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORKatrinFLTWaveformTreeWriter::ORKatrinFLTWaveformTreeWriter(string treeName) :
ORVTreeWriter(new ORKatrinFLTWaveformDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORKatrinFLTWaveformDecoder*>(fDataDecoder);
  Clear();
  fWaveform[0] = 0;  // this is on the stack, not the heap.
}

ORKatrinFLTWaveformTreeWriter::~ORKatrinFLTWaveformTreeWriter()
{
  delete fEventDecoder;
}

/** Create the ROOT branches.
  * 
  */ //-tb- 2008-02-12
ORDataProcessor::EReturnCode ORKatrinFLTWaveformTreeWriter::InitializeBranches()
{
  fTree->Branch("wfLength", &fWaveformLength, "wfLength/I");
  fTree->Branch("eventSec", &fSec, "eventSec/i");
  fTree->Branch("eventSubSec", &fSubSec, "eventSubSec/i");
  fTree->Branch("eventID", &fEventID, "eventID/s");
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("channel", &fChannel, "channel/s");
  fTree->Branch("channelMap", &fChannelMap, "channelMap/i");
  fTree->Branch("pageNumber", &fPageNumber, "pageNumber/s");
  fTree->Branch("energy", &fEnergy, "energy/i");
  fTree->Branch("resetSec", &fResetSec, "resetSec/i");
  fTree->Branch("resetSubSec", &fResetSubSec, "resetSubSec/i");
  fTree->Branch("waveform", fWaveform, "waveform[wfLength]/s");
  return kSuccess;
}

/**  This sets the data/variables for the ROOT branches.
  *  TTree::Fill() is called in ORBasicTreeWriter::ProcessMyDataRecord(UInt_t* record) (?)
  *  This class is a ORDataProcessor and ORVTreeWriter, NOT a ORBasicTreeWriter!
  *  (See comments in ORDataProcessor.hh and ORVTreeWriter.hh)
  */ //-tb- 2008-02-12
ORDataProcessor::EReturnCode ORKatrinFLTWaveformTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
  if(!fEventDecoder->SetDataRecord(record)) return kFailure;
      // check severity to improve speed:
  fSec = fEventDecoder->GetSec();
  fSubSec = fEventDecoder->GetSubSec();
  fEventID = fEventDecoder->GetEventID();
  fEnergy = fEventDecoder->GetEnergy();
  fCrate = fEventDecoder->CrateOf();
  fCard = fEventDecoder->CardOf();
  fChannel = fEventDecoder->GetChannel();
  fChannelMap = fEventDecoder->GetChannelMap();
  fPageNumber = fEventDecoder->GetPageNumber();
  fWaveformLength = fEventDecoder->GetWaveformLen();
  fResetSec = fEventDecoder->GetResetSec();
  fResetSubSec = fEventDecoder->GetResetSubSec();
   
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "event-sec-subsec-crate-card-channel-energy-resetsec-resetsubsec-chmap-pagenum = "
      << fEventID << "-" << fSec << "-" << fSubSec << "-" << fCrate << "-"
      << fCard << "-" << fChannel << "-" << fEnergy 
      << "-" << fResetSec << "-" << fResetSubSec   //-tb- 2008-02-12
      << "-" << fChannelMap << "-" << fPageNumber  //-tb- 2008-02-12
      << endl;
  }
  if(fWaveformLength > kMaxWFLength) {
    ORLog(kError) << "Waveform length (" << fWaveformLength 
      << ") exceeds kMaxWFLength (" << kMaxWFLength << ")" << endl;
    return kFailure;
  }
  
  memcpy(fWaveform, fEventDecoder->GetWaveformDataPointer(), 
    fWaveformLength*sizeof(UShort_t));
	
  // Swap the data when reading by a big endian machine
  // ak 6.3.08
  // ntohl is non-standard or at least resides in non-standard
  // header files.  Use ORBIG_ENDIAN_MACHINE instead
  // FixME ALL swapping should be performed by the decoder's Swap() function
  // since the processor should not need to know about the
  // structure of the data. 
#ifdef ORBIG_ENDIAN_MACHINE 
//  if (ntohl(1) == 1){ // big endian host
    UInt_t *ptr = (UInt_t *) fWaveform; 
		
	for (int i=0;i<fWaveformLength/2;i++)
	  ptr[i] = (ptr[i] >> 16)  |  (ptr[i] << 16);
//  }
#endif
	
  return kSuccess;
}

/**  End decoding the current file. Writes the tree to the output file.
  *  This is almost an exact copy of the original ORVTreeWriter::EndRun()
  *  except that empty trees are not written to the root file if the flag 
  *  saveOnlyNonemptyTrees is set to @e true.
  *
  * @todo Should write getter and setter for saveOnlyNonemptyTrees
  */   // -tb- 2008-02-19
  // TODO: Should write getter and setter for saveOnlyNonemptyTrees
ORDataProcessor::EReturnCode ORKatrinFLTWaveformTreeWriter::EndRun()
{
  // Write the tree only if this processor is meant to write it.
  // fUniqueTree implies that the tree was manually filled; it still should
  // be written automatically. A manual write can be performed by
  // overloading this function.
  if (fThisProcessorAutoFillsTree || fUniqueTree) {
    if (fFillMethod == kFillBeforeProcessDataRecord && fLastProcessedRecordRetCode == kSuccess) fTree->Fill();

    if (fTree->GetEntries() == 0) {
      ORLog(kWarning) << "EndRun(): no entries in fTree XXX" << fTree->GetName() << endl;
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

