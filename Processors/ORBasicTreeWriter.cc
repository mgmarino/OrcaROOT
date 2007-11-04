// ORBasicTreeWriter.cc

#include "ORBasicTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORBasicTreeWriter::ORBasicTreeWriter(ORVBasicTreeDecoder* decoder, string treeName, string branchPrefix) :
ORVTreeWriter(decoder, treeName)
{
  fBasicTreeDecoder = decoder;
  fBranchPrefix = branchPrefix;
  // ORBasicTreeWriters don't leave the filling of the tree to
  // ORVTreeWriter. For example, when a record has multiple rows to
  // write to a tree, each one must be written in sequence during
  // ProcessMyDataRecord().  For this reason, ORBasicTreeWriters write
  // to a unique tree and consist use a single decoder. If you require
  // a more complex tree filling, then you need something more than a
  // "basic" tree.
  SetDoNotAutoFillTree();
}

ORBasicTreeWriter::~ORBasicTreeWriter()
{
  for (unsigned i=0; i<fParameters.size(); i++) delete fParameters[i];
}

ORDataProcessor::EReturnCode ORBasicTreeWriter::InitializeBranches()
{
  ORLog(kTrace) << "Initializing branches..." << endl;
  for (size_t iPar=0; iPar<fBasicTreeDecoder->GetNPars(); iPar++) {
    fParameters.push_back(new UInt_t);
    fTree->Branch(
      (fBranchPrefix + fBasicTreeDecoder->GetParName(iPar)).c_str(),
      fParameters[iPar], 
      (fBranchPrefix + fBasicTreeDecoder->GetParName(iPar)+"/i").c_str()
    );
  }
  ORLog(kTrace) << "Initialized " << fBasicTreeDecoder->GetNPars() << " branches." << endl;
  return kSuccess;
}

ORDataProcessor::EReturnCode ORBasicTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  for (size_t iRow=0; iRow<fBasicTreeDecoder->GetNRows(record); iRow++) {
    for (size_t iPar=0; iPar<fBasicTreeDecoder->GetNPars(); iPar++) {
      *(fParameters[iPar]) = fBasicTreeDecoder->GetPar(record, iPar, iRow);
      if(ORLogger::GetSeverity() <= ORLogger::kDebug) {
        ORLog(kDebug) << fBranchPrefix+fBasicTreeDecoder->GetParName(iPar) << ": " 
                      << *(fParameters[iPar]) << endl;
      }
    }
    fTree->Fill();
  }
  return kSuccess;
}

void ORBasicTreeWriter::Clear()
{
  ORLog(kWarning) << "You should never have to use ORBasicTreeWriter::Clear(), "
                  << "since ORBasicTreeWriter writes to a unique tree" << endl;
  for (size_t iPar=0; iPar<fBasicTreeDecoder->GetNPars(); iPar++) {
    (*(fParameters[iPar])) = 0;
  }
}

