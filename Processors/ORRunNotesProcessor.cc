// ORRunNotesProcessor.cc

#include "ORRunNotesProcessor.hh"

#include "TROOT.h"
#include "TObjString.h"
#include "ORHeader.hh"
#include "ORRunContext.hh"

ORDataProcessor::EReturnCode ORRunNotesProcessor::StartRun()
{
  /* Here we explicitly check if we have at least a reasonable buffer length. */
  const ORHeader* header = fRunContext->GetHeader();
  const ORDictionary* adict = dynamic_cast<const ORDictionary*>(header->LookUp("RunNotes"));
  if (!adict) return kFailure;
  // Make a directory to store the information
  gDirectory->mkdir("RunNotes");
  gDirectory->cd("RunNotes");

  const ORDictionary::DictMap& mapdict = adict->GetDictMap();
  ORDictionary::DictMap::const_iterator iter = mapdict.begin(); 
  for( ;iter != mapdict.end();iter++) {
    TObjString tempStr(iter->second->GetStringOfValue().c_str()); 
    tempStr.Write(iter->first.c_str());
  }
  gDirectory->cd(0);

  return kSuccess;
}
