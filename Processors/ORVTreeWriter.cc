// ORVTreeWriter.cc

#include "ORVTreeWriter.hh"

#include "TROOT.h"
#include "ORLogger.hh"
#include "ORRunContext.hh"

using namespace std;

ORVTreeWriter::ORVTreeWriter(ORVDataDecoder* decoder, string treeName) :
ORDataProcessor(decoder)
{
  fTreeName = treeName;
  if(fTreeName == "") fTreeName = "ORTree";
  fTree = NULL;
  fThisProcessorAutoFillsTree = true;
  fUniqueTree = false;
  fFillPeriod = 1;
  fFillMethod = kFillAfterProcessDataRecord;
  fFillCount = 0;
  fLastProcessedRecordRetCode = kSuccess;
  fSaveOnlyNonemptyTrees = false;  // I would prefer 'true', but this (false) will keep the ancient behaviour -tb- 2008-07-25
}

ORDataProcessor::EReturnCode ORVTreeWriter::StartRun()
{
  EReturnCode retCode = InitializeTree();
  if (retCode >= kAlarm) return retCode;

  fFillCount = 0;
  if(fFillPeriod < 1) {
    ORLog(kWarning) << "StartRun(): This processor is supposed "
                    << "to fill the tree, but the fill period has been "
                    << "set to be < 1 (=" << fFillPeriod << "); " 
		    << "will set to 1" << endl;
    fFillPeriod = 1;
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORVTreeWriter::ProcessDataRecord(UInt_t* record)
{
  if (!fDoProcess || !fDoProcessRun || !fRunContext) return kFailure;
  if (fDataDecoder->DataIdOf(record) != fDataId) return kSuccess;
  if(fRunContext->MustSwap() && !fRunContext->IsRecordSwapped()) {
    /* Swapping the record.  This only must be done once! */
    fDataDecoder->Swap(record);
    fRunContext->SetRecordSwapped();
  }

  if (fThisProcessorAutoFillsTree && 
      fFillMethod == kFillBeforeProcessDataRecord &&
      fLastProcessedRecordRetCode == kSuccess) {
    if (fFillCount > 0 && fFillCount % fFillPeriod == 0) fTree->Fill();
    fFillCount++;
  }

  if(fDebugRecord) fDataDecoder->DumpHex(record);

  fLastProcessedRecordRetCode = ProcessMyDataRecord(record);
  if (fLastProcessedRecordRetCode >= kAlarm) return fLastProcessedRecordRetCode;

  if (fThisProcessorAutoFillsTree && 
      fFillMethod == kFillAfterProcessDataRecord &&
      fLastProcessedRecordRetCode == kSuccess) {
    fFillCount++;
    if (fFillCount % fFillPeriod == 0) fTree->Fill();
  }
  return fLastProcessedRecordRetCode;
}

/**  End decoding the current file. Writes the tree to the output file.
  *
  *  Empty trees are not written to the root file if the flag 
  *  fSaveOnlyNonemptyTrees is set to @e true.
  *  (This behaviour was implemented for ORKatrinFLTEnergyTreeWriter and ORKatrinFLTWaveformTreeWriter 
  *  and was moved back to the base class ORVTreeWriter in 2008-07-25.)
  *  
  */   // -tb- 2008-02-19
ORDataProcessor::EReturnCode ORVTreeWriter::EndRun()
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

    // -tb- original code: fTree->Write(fTree->GetName(), TObject::kOverwrite);
    if(fSaveOnlyNonemptyTrees){// -tb- 2008-02-19 - moved to base class ORVTreeWriter 2008-07-25
      if (fTree->GetEntries() != 0) fTree->Write(fTree->GetName(), TObject::kOverwrite);
      else ORLog(kWarning) << "EndRun(): did not write empty fTree " << fTree->GetName() << endl;

    }else{
       fTree->Write(fTree->GetName(), TObject::kOverwrite);
    }
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORVTreeWriter::InitializeTree()
{
  // check if the tree already exists -- this way, several processors can
  // write to the same tree. 
  fTree = (TTree*) gDirectory->FindObject(fTreeName.c_str()); 

  // If the tree is supposed to be unique, keep looking until you get a
  // tree with a unique name. fTree is left NULL so that a new one will be
  // made in the next code block.
  if(fUniqueTree) {
    int i=2;
    string treeNameBase = fTreeName;
    while(fTree != NULL) {
      SetTreeName(treeNameBase + Form("_%d", i++));
      fTree = (TTree*) gDirectory->FindObject(fTreeName.c_str()); 
    }
  }

  // If fTree is still NULL by this point, need to make a new one.
  if(fTree == NULL) {
    if(gROOT->GetListOfFiles()->GetSize() == 0) {
      ORLog(kWarning) << "InitializeTree(): no root file open. " 
                      << "Building tree " << fTreeName
		      << " in gDirectory = gROOT, will not be saved..." 
		      << endl;
    }

    // fTree is owned by parent root file (or gROOT); 
    // it will be deleted upon fFile->Close() (or end of program).
    // So there is no delete associated with this call to new.
    fTree = new TTree(fTreeName.c_str(), fTreeName.c_str()); 

    if (!fRunContext) {
      ORLog(kError) << "fRunContext is NULL!" << endl;
      return kFailure;
    }
    // always save the run number, the sub-run number, 
    // and the state of the run
    fTree->Branch("runNumber", fRunContext->GetPointerToRunNumber(), "runNumber/I");
    fTree->Branch("subRunNumber", fRunContext->GetPointerToSubRunNumber(), "subRunNumber/I");
    fTree->Branch("runningState", fRunContext->GetPointerToStringOfState());
  }

  return InitializeBranches();
}

void ORVTreeWriter::SetThisProcessorAutoFillsTree(bool fillTree)
{
  if(fillTree && fUniqueTree) {
    // fUniqueTree == true means that SetDoNotAutoFillTree was called, so
    // this function shouldn't be called!
    ORLog(kWarning) << "SetThisProcessorAutoFillsTree(true): This processor "
                    << "does not autofill, ignoring." << endl;
    return;
  }
  fThisProcessorAutoFillsTree = fillTree;
}

void ORVTreeWriter::SetDoNotAutoFillTree()
{
  if(fTree != NULL) {
    ORLog(kWarning) << "SetDoNotAutoFillTree(): this function should be"
                    << "called in the constructor of a derived class that "
		    << "fills its tree manually. You seem to have called "
		    << "it after the tree has been made! That is a bad idea." 
		    << endl;
  }
  if(fUniqueTree == true) {
    ORLog(kWarning) << "SetDoNotAutoFillTree(): fUniqueTree was already true. "
                    << "Did you try to call this function twice?" << endl;
  }
  SetThisProcessorAutoFillsTree(false); // never call fTree->Fill() within ORVTreeWriter.cc
  fUniqueTree = true; 
}
