// ORTrigger32TreeWriter.cc

#include "ORTrigger32ShaperTreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORTrigger32ShaperTreeWriter::ORTrigger32ShaperTreeWriter(string treeBaseName)
{
  /*string treeName = treeBaseName + "1Tree";
  fTriggerGTID1TreeWriter = new ORTrigger32GTIDTreeWriter(&fTriggerGTID1Decoder, treeName);
  fTriggerGTID1TreeWriter->SetThisProcessorAutoFillsTree(true);
  fTriggerGTID1TreeWriter->SetFillMethod(ORVTreeWriter::kFillBeforeProcessDataRecord);
  AddProcessor(fTriggerGTID1TreeWriter);
  fTrigger1ClockTreeWriter = new ORTrigger32ClockTreeWriter(treeName);
  fTrigger1ClockTreeWriter->SetThisProcessorAutoFillsTree(false);
  fTrigger1ClockTreeWriter->SetTriggerBitsFilter(ORTrigger32ClockTreeWriter::kTrigger1); 
  AddProcessor(fTrigger1ClockTreeWriter);*/
//Shaper record is trigger 2 event.
  treeName = treeBaseName + "2Tree";
  fTriggerGTID2TreeWriter = new ORTrigger32GTIDTreeWriter(&fTriggerGTID2Decoder, treeName);
  fTriggerGTID2TreeWriter->SetThisProcessorAutoFillsTree(false);
  AddProcessor(fTriggerGTID2TreeWriter);
  fTrigger2ClockTreeWriter = new ORTrigger32ClockTreeWriter(treeName);
  fTrigger2ClockTreeWriter->SetThisProcessorAutoFillsTree(false);
  fTrigger2ClockTreeWriter->SetTriggerBitsFilter(ORTrigger32ClockTreeWriter::kTrigger2); 
  AddProcessor(fTrigger2ClockTreeWriter);
  fshaperShaperTreeWriter = new ORShaperShaperTreeWriter(treeName);
  fshaperShaperTreeWriter->SetThisProcessorAutoFillsTree(true);
  fshaperShaperTreeWriter->SetFillMethod(ORVTreeWriter::kFillAfterProcessDataRecord);
  AddProcessor(fshaperShaperTreeWriter);
  //Live Time Records
  treeName = treeBaseName + "LTTree";
  fTriggerLiveTimeTreeWriter = new ORBasicTreeWriter(&fTriggerLiveTimeDecoder, treeName);
  AddProcessor(fTriggerLiveTimeTreeWriter);
}

ORTrigger32ShaperTreeWriter::~ORTrigger32ShaperTreeWriter()
{
  for(size_t i=0; i < fDataProcessors.size(); i++) {
    delete fDataProcessors[i];
  }
}

