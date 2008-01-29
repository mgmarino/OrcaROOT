// OROrcaRequestProcessor.cc

#include "OROrcaRequestProcessor.hh"
#include "ORDebugRequestProcessor.hh"
#include "OROrcaRequestFitProcessor.hh"
#include "OROrcaRequestFFTProcessor.hh"

#include "TROOT.h"
#include "ORLogger.hh"
#include "ORDictionary.hh"
#include "ORXmlPlistString.hh"
#include "ORUtils.hh"
#include "TSocket.h"

using namespace std;

OROrcaRequestProcessor::OROrcaRequestProcessor() : ORDataProcessor(new OROrcaRequestDecoder)
{
  fOrcaRequestDecoder = dynamic_cast<OROrcaRequestDecoder*>(fDataDecoder);
}

OROrcaRequestProcessor::~OROrcaRequestProcessor()
{
  delete fOrcaRequestDecoder;
  std::map<std::string, ORVOrcaRequestProcessor*>::iterator iter = 
    fReqProcessorMap.begin();
  while(iter != fReqProcessorMap.end()) {
    delete iter->second;
    iter++;
  }
}

bool OROrcaRequestProcessor::LoadRequestHandler(const std::string& processor)
{
  // First check to see if the Processor is already in the map.
  std::map<std::string, ORVOrcaRequestProcessor*>::iterator iter;
  iter = fReqProcessorMap.find(processor); 
  ORLog(kRoutine) << "Loading processor: " << processor << endl;
  if(iter != fReqProcessorMap.end()) {
    fCurrentReqProcessor = iter->second;
    return true;
  }
  // OK so we have to load it:
  if(processor=="ORDebugRequestProcessor") {
    // load the processor like:
    fCurrentReqProcessor = new ORDebugRequestProcessor;
    fReqProcessorMap[processor] = fCurrentReqProcessor;
    return true;
  }  else if (processor=="OROrcaRequestFitProcessor") {
    fCurrentReqProcessor = new OROrcaRequestFitProcessor;
    fReqProcessorMap[processor] = fCurrentReqProcessor;
    return true;
#ifdef ORROOT_HAS_FFTW
  }  else if (processor=="OROrcaRequestFFTProcessor") {
    fCurrentReqProcessor = new OROrcaRequestFFTProcessor;
    fReqProcessorMap[processor] = fCurrentReqProcessor;
    return true;
#endif
  } else {
    ORLog(kError) << processor << " not available. " << endl;
    fCurrentReqProcessor = NULL;
    return false; 
  }
}

bool OROrcaRequestProcessor::LoadInputs()
{
  const ORDictionary* inputDict; 
  const std::map< std::string, ORVOrcaReqInputOutput>* mapOfInput; 
  std::map< std::string, ORVOrcaReqInputOutput>::const_iterator inputIter; 
  inputDict = fOrcaRequestDecoder->GetRequestInputs();
  mapOfInput = fCurrentReqProcessor->GetInputMap();
  inputIter = mapOfInput->begin();
  ORLog(kDebug) << "Loading of inputs beginning..." << endl; 
  while(inputIter != mapOfInput->end()) {
    const ORVDictValue* dictVal = inputDict->LookUp(inputIter->first);
    if(!dictVal) {
      ORLog(kError) << "Error finding " << inputIter->first << " in inputs. "
        << endl;
      return false;
    }
    if(dictVal->GetValueType() == ORVDictValue::kString) {
      ORLog(kDebug) << "Inserting input " << inputIter->first << " into "
        << fCurrentReqProcessor->GetNameOfRequestProcessor() << endl;
      fCurrentReqProcessor->LoadInput(inputIter->first, 
        &(dynamic_cast<const ORDictValueS*>(dictVal)->GetS())); 
    }
    else if(dictVal->GetValueType() == ORVDictValue::kReal) {
      ORLog(kDebug) << "Inserting input " << inputIter->first << " into "
        << fCurrentReqProcessor->GetNameOfRequestProcessor() << endl;
      fCurrentReqProcessor->LoadInput(inputIter->first, 
        &(dynamic_cast<const ORDictValueR*>(dictVal)->GetR())); 
    }
    else if(dictVal->GetValueType() == ORVDictValue::kInt) {
      ORLog(kDebug) << "Inserting input " << inputIter->first << " into "
        << fCurrentReqProcessor->GetNameOfRequestProcessor() << endl;
      fCurrentReqProcessor->LoadInput(inputIter->first, 
        &(dynamic_cast<const ORDictValueI*>(dictVal)->GetI())); 
    }
    else if(dictVal->GetValueType() == ORVDictValue::kArray) {
      ORLog(kDebug) << "Inserting input " << inputIter->first << " into "
        << fCurrentReqProcessor->GetNameOfRequestProcessor() << endl;
      const ORDictValueA* dictArray = dynamic_cast<const ORDictValueA*>(dictVal);
      fCurrentReqProcessor->LoadInput(inputIter->first, dictArray);
    } else {
      ORLog(kError) << "Incorrect type read in for: " << inputIter->first 
        << endl;
      return false;
    }
    
    inputIter++;
  }
  ORLog(kDebug) << "Loading of inputs complete." << endl; 
  return true;
}

bool OROrcaRequestProcessor::ExecuteProcess()
{
  ORLog(kRoutine) << "Beginning Process execution... " << endl;
  bool tmpBool = fCurrentReqProcessor->ExecuteProcess(); 
  if(tmpBool) ORLog(kRoutine) << "Process execution complete... " << endl;
  else ORLog(kError) << "Process execution failed... " << endl;
  return tmpBool; 
}

bool OROrcaRequestProcessor::LoadOutputs()
{
  /* A lot of copying happens here and this is mainly to keep the interface clean.
   * It could be done more quickly, but we're not as concerned about speed for the
   * Orca Socket. We have to assemble the output into a dictionary. */
  ORLog(kDebug) << "Getting outputs to submit to Orca..." << endl;
  const std::map< std::string, ORVOrcaReqInputOutput>* mapOfOutput; 
  std::map< std::string, ORVOrcaReqInputOutput>::const_iterator mapOfOutputIter; 
  mapOfOutput = fCurrentReqProcessor->GetOutputMap();
  
  /* We need to now set up the output into ORDictionaries. */
  /* Memory management is handled by ORDictionary itself. */ 
  ORDictionary rootOutputDict("OrcaRequestOutput");
  rootOutputDict.LoadEntry("Request Tag Number", 
    new ORDictValueI(fOrcaRequestDecoder->GetRequestTag()));
  rootOutputDict.LoadEntry("Request Option",
    new ORDictValueS(fOrcaRequestDecoder->GetRequestOption()));
  rootOutputDict.LoadEntry("Request Type",
    new ORDictValueS(fOrcaRequestDecoder->GetRequestType()));
  ORDictionary* outputDict = new ORDictionary("Request Outputs"); 
  
  /* now loading in the output. */
  for(mapOfOutputIter=mapOfOutput->begin();
    mapOfOutputIter!=mapOfOutput->end();mapOfOutputIter++) {
    ORVOrcaRequestProcessor::EORVOrcaRequestProcessorConsts theType = 
      mapOfOutputIter->second.type;
    ORVDictValue* inputDictValue = NULL;
    switch (theType) {
      case ORVOrcaRequestProcessor::kString: {
          inputDictValue = 
            new ORDictValueS(*(std::string*)mapOfOutputIter->second.varAddress); 
        }
        break;
      case ORVOrcaRequestProcessor::kInt: {
          inputDictValue = 
            new ORDictValueI(*(int*)mapOfOutputIter->second.varAddress); 
        }
        break;
      case ORVOrcaRequestProcessor::kReal: {
          inputDictValue = 
            new ORDictValueR(*(double*)mapOfOutputIter->second.varAddress); 
        }
        break;
      case ORVOrcaRequestProcessor::kStringVec: {
          inputDictValue = new ORDictValueA(); 
          std::vector<std::string>* vecS = 
            (std::vector<std::string>*)mapOfOutputIter->second.varAddress; 
          for(size_t i=0;i<vecS->size();i++) {
            ((ORDictValueA*)inputDictValue)->LoadValue(new ORDictValueS((*vecS)[i]));
          }
        }
        break;
      case ORVOrcaRequestProcessor::kIntVec: {
          inputDictValue = new ORDictValueA(); 
          std::vector<int>* vecI = 
            (std::vector<int>*)mapOfOutputIter->second.varAddress; 
          for(size_t i=0;i<vecI->size();i++) {
            ((ORDictValueA*)inputDictValue)->LoadValue(new ORDictValueI((*vecI)[i]));
          }
        }
        break;
      case ORVOrcaRequestProcessor::kRealVec: {
          inputDictValue = new ORDictValueA(); 
          std::vector<double>* vecR = 
            (std::vector<double>*)mapOfOutputIter->second.varAddress; 
          for(size_t i=0;i<vecR->size();i++) {
            ((ORDictValueA*)inputDictValue)->LoadValue(new ORDictValueR((*vecR)[i]));
          }
        }
        break;
    }
    outputDict->LoadEntry(mapOfOutputIter->first, inputDictValue);  

  }
  rootOutputDict.LoadEntry(outputDict->GetName(), outputDict);
  ORXmlPlistString xmlOutput; 
  xmlOutput.LoadDictionary(&rootOutputDict);
 
  /* Now send along the xml list back to orca */ 
  if (!fRunContext) return false;
  TSocket* lastSock = fRunContext->GetWritableSocket(); 
  if(!lastSock) {
    ORLog(kError) << "No socket found to write back on.  Was this header read in as a file?" << endl; 
    ORLog(kDebug) << "Outputting xml: " << endl << xmlOutput << endl;
    return false;
  }

  /* The data id is gotten and or-ed with the length to resend 
   * as the first word.  Dataid's are only in the upper 16 bits
   * of the 32 bit word. The length is in number of 4-byte words.*/
  while(xmlOutput.length() % 4 != 0) xmlOutput.append(" ");
  UInt_t dataIdToResend = fOrcaRequestDecoder->GetDataId();
  dataIdToResend |= xmlOutput.length()/4 + 1;
 
  if(fRunContext->MustSwap()) ORUtils::Swap(dataIdToResend);
  /* We have to swap the binary, but not the char data*/
  
  ORLog(kDebug) << "Submitting outputs back to Orca..." << endl;
  int nBytesRead = lastSock->SendRaw((char*)&dataIdToResend, 4);
  if(nBytesRead==4) {
    nBytesRead = lastSock->SendRaw(xmlOutput.c_str(), xmlOutput.length());
    if(nBytesRead <= 0) return false;
    else return true;
  } else return false;    
}

void OROrcaRequestProcessor::SendErrorToOrca()
{
  /* Send an error to Orca since something went wrong. */
  ORLog(kDebug) << "Sending error tag back to Orca." << endl;
  ORDictionary rootErrorDict("OrcaRequestOutput");
  rootErrorDict.LoadEntry("Request Tag Number", 
    new ORDictValueI(fOrcaRequestDecoder->GetRequestTag()));
  rootErrorDict.LoadEntry("Request Option",
    new ORDictValueS(fOrcaRequestDecoder->GetRequestOption()));
  rootErrorDict.LoadEntry("Request Type",
    new ORDictValueS(fOrcaRequestDecoder->GetRequestType()));

  rootErrorDict.LoadEntry("Request Error", new ORDictValueS("Error"));
  ORXmlPlistString xmlOutput; 
  xmlOutput.LoadDictionary(&rootErrorDict);
  if (!fRunContext) return;
  TSocket* lastSock = fRunContext->GetWritableSocket(); 
  if(!lastSock) {
    ORLog(kError) << "No socket found to write back on.  Was this header read in as a file?" << endl; 
    ORLog(kDebug) << "Outputting xml: " << endl << xmlOutput << endl;
    return;
  }
  /* The data id is gotten and or-ed with the length to resend 
   * as the first word.  Dataid's are only in the upper 16 bits
   * of the 32 bit word. The length is in number of 4-byte words.*/
  while(xmlOutput.length() % 4 != 0) xmlOutput.append(" ");
  UInt_t dataIdToResend = fOrcaRequestDecoder->GetDataId();
  dataIdToResend |= xmlOutput.length()/4 + 1;
  if(fRunContext->MustSwap()) ORUtils::Swap(dataIdToResend);
  /* We have to swap the binary, but not the char data*/

  lastSock->SendRaw((char*)&dataIdToResend, 4);
  lastSock->SendRaw(xmlOutput.c_str(), xmlOutput.length());
}

bool OROrcaRequestProcessor::ExecuteAll(UInt_t* record)
{
  if(!fOrcaRequestDecoder->ParseDataRecord(record)) return false;
  if(!LoadRequestHandler(fOrcaRequestDecoder->GetRequestType())) return false; 
  if(!LoadInputs()) return false;
  if(!ExecuteProcess()) return false;
  if(!LoadOutputs()) return false;
  return true;
}

ORDataProcessor::EReturnCode OROrcaRequestProcessor::ProcessMyDataRecord(UInt_t* record)
{
  if(!ExecuteAll(record)) {
    SendErrorToOrca();
    return kFailure;
  } else return kSuccess;
}

