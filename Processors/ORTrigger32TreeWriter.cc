// ORTrigger32TreeWriter.cc

#include "ORTrigger32TreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORTrigger32TreeWriter::ORTrigger32TreeWriter(string treeBaseName)
{
  string treeName = treeBaseName + "1Tree";
  fTriggerGTID1TreeWriter = new ORTrigger32GTIDTreeWriter(&fTriggerGTID1Decoder, treeName);
  fTriggerGTID1TreeWriter->SetThisProcessorAutoFillsTree(true);
  fTriggerGTID1TreeWriter->SetFillMethod(ORVTreeWriter::kFillBeforeProcessDataRecord);
  AddProcessor(fTriggerGTID1TreeWriter);
  fTrigger1ClockTreeWriter = new ORTrigger32ClockTreeWriter(treeName);
  fTrigger1ClockTreeWriter->SetThisProcessorAutoFillsTree(false);
  fTrigger1ClockTreeWriter->SetTriggerBitsFilter(ORTrigger32ClockTreeWriter::kTrigger1); 
  AddProcessor(fTrigger1ClockTreeWriter);

  treeName = treeBaseName + "2Tree";
  fTriggerGTID2TreeWriter = new ORTrigger32GTIDTreeWriter(&fTriggerGTID2Decoder, treeName);
  fTriggerGTID2TreeWriter->SetThisProcessorAutoFillsTree(true);
  fTriggerGTID2TreeWriter->SetFillMethod(ORVTreeWriter::kFillBeforeProcessDataRecord);
  AddProcessor(fTriggerGTID2TreeWriter);
  fTrigger2ClockTreeWriter = new ORTrigger32ClockTreeWriter(treeName);
  fTrigger2ClockTreeWriter->SetThisProcessorAutoFillsTree(false);
  fTrigger2ClockTreeWriter->SetTriggerBitsFilter(ORTrigger32ClockTreeWriter::kTrigger2); 
  AddProcessor(fTrigger2ClockTreeWriter);

  treeName = treeBaseName + "LTTree";
  fTriggerLiveTimeTreeWriter = new ORBasicTreeWriter(&fTriggerLiveTimeDecoder, treeName);
  AddProcessor(fTriggerLiveTimeTreeWriter);
}

ORTrigger32TreeWriter::~ORTrigger32TreeWriter()
{
  for(size_t i=0; i < fDataProcessors.size(); i++) {
    delete fDataProcessors[i];
  }
}

