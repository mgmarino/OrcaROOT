// ORVOrcaRequestProcessor.cc

#include "ORVOrcaRequestProcessor.hh"

#include "TROOT.h"
#include "ORLogger.hh"

using namespace std;

ORVOrcaRequestProcessor::ORVOrcaRequestProcessor() 
{
}

ORVOrcaRequestProcessor::~ORVOrcaRequestProcessor()
{
}


bool ORVOrcaRequestProcessor::LoadInput(const std::string& varName, 
  const std::string* inputString, size_t sizeOfParam )
{
  fInputMapIter = fInputMap.find(varName);
  if(fInputMapIter == fInputMap.end()) {
    ORLog(kError) << varName << " is not a registered input of: " 
      << GetNameOfRequestProcessor() << endl;
    return false;
  }
  if(fInputMapIter->second.type != kString && 
    fInputMapIter->second.type != kStringVec) {
    ORLog(kError) << "String invalid type for: " << varName << " in "
      << GetNameOfRequestProcessor() << endl;
    return false;
  }
  if(fInputMapIter->second.type == kString) {
    *((std::string*)fInputMapIter->second.varAddress) = *inputString;
    return true;
  }
  else {
    std::vector<std::string>* stringVec = 
      ((std::vector<std::string>*)fInputMapIter->second.varAddress);
    stringVec->resize(sizeOfParam);
    stringVec->assign(inputString, inputString + sizeOfParam);
    return true;
  }
    
}

bool ORVOrcaRequestProcessor::LoadInput(const std::string& varName, 
  const int* inputInteger, size_t sizeOfParam )
{
  fInputMapIter = fInputMap.find(varName);
  if(fInputMapIter == fInputMap.end()) {
    ORLog(kError) << varName << " is not a registered input of: " 
      << GetNameOfRequestProcessor() << endl;
    return false;
  }
  if(fInputMapIter->second.type != kInt && fInputMapIter->second.type != kIntVec) {
    ORLog(kError) << "String invalid type for: " << varName << " in "
      << GetNameOfRequestProcessor() << endl;
    return false;
  }
  if(fInputMapIter->second.type == kInt) {
    *((int*)fInputMapIter->second.varAddress) = *inputInteger;
    return true;
  }
  else {
    std::vector<int>* intVec = 
      ((std::vector<int>*)fInputMapIter->second.varAddress);
    intVec->resize(sizeOfParam);
    intVec->assign(inputInteger, inputInteger + sizeOfParam);
    return true;
  }
    
}

bool ORVOrcaRequestProcessor::LoadInput(const std::string& varName, 
  const double* inputDouble, size_t sizeOfParam )
{
  fInputMapIter = fInputMap.find(varName);
  if(fInputMapIter == fInputMap.end()) {
    ORLog(kError) << varName << " is not a registered input of: " 
      << GetNameOfRequestProcessor() << endl;
    return false;
  }
  if(fInputMapIter->second.type != kReal && fInputMapIter->second.type != kRealVec) {
    ORLog(kError) << "String invalid type for: " << varName << " in "
      << GetNameOfRequestProcessor() << endl;
    return false;
  }
  if(fInputMapIter->second.type == kReal) {
    *((double*)fInputMapIter->second.varAddress) = *inputDouble;
    return true;
  }
  else {
    std::vector<double>* doubleVec = 
      ((std::vector<double>*)fInputMapIter->second.varAddress);
    doubleVec->resize(sizeOfParam);
    doubleVec->assign(inputDouble, inputDouble + sizeOfParam);
    return true;
  }
    
}

bool ORVOrcaRequestProcessor::LoadInput(const std::string& varName, 
  const ORDictValueA* dictArray) 
{
  fInputMapIter = fInputMap.find(varName);
  if(fInputMapIter == fInputMap.end()) {
    ORLog(kError) << varName << " is not a registered input of: " 
      << GetNameOfRequestProcessor() << endl;
    return false;
  }
  if(fInputMapIter->second.type != kRealVec 
     && fInputMapIter->second.type != kIntVec 
     && fInputMapIter->second.type != kStringVec) {
    ORLog(kError) << "Invalid type for: " << varName << " in "
      << GetNameOfRequestProcessor() << endl;
    return false;
  }
  if(dictArray->GetNValues()==0) return true;
  /* Not loading anything*/

  ORVDictValue::EValType type = dictArray->At(0)->GetValueType();
  switch(type) {
    case ORVDictValue::kString: {
        if(fInputMapIter->second.type != kStringVec) {
          ORLog(kError) << "String type needed for array " 
          << varName << endl;
          return false;
        }
        std::vector<std::string>* stringVec = 
          ((std::vector<std::string>*)fInputMapIter->second.varAddress);
        stringVec->resize(dictArray->GetNValues());
        for(size_t i=0;i<dictArray->GetNValues();i++) {
          (*stringVec)[i] = 
            dynamic_cast<const ORDictValueS*>(dictArray->At(i))->GetS();
        }
      }
      break;
    case ORVDictValue::kInt: {
        if(fInputMapIter->second.type != kIntVec) {
          ORLog(kError) << "Int type needed for array " 
          << varName << endl;
          return false;
        }
        std::vector<int>* intVec = 
          ((std::vector<int>*)fInputMapIter->second.varAddress);
        intVec->resize(dictArray->GetNValues());
        for(size_t i=0;i<dictArray->GetNValues();i++) {
          (*intVec)[i] = 
            dynamic_cast<const ORDictValueI*>(dictArray->At(i))->GetI();
        }
      }
      break;
    case ORVDictValue::kReal: {
        if(fInputMapIter->second.type != kRealVec) {
          ORLog(kError) << "Real type needed for array " 
          << varName << endl;
          return false;
        }
        std::vector<double>* doubleVec = 
          ((std::vector<double>*)fInputMapIter->second.varAddress);
        doubleVec->resize(dictArray->GetNValues());
        for(size_t i=0;i<dictArray->GetNValues();i++) {
          (*doubleVec)[i] = 
            dynamic_cast<const ORDictValueR*>(dictArray->At(i))->GetR();
        }
      }
      break;
    default: {
        ORLog(kDebug) << "Unknown type: " << type << endl;
        return false;
      }
      break;
  }
   
  return true;
    
}

void ORVOrcaRequestProcessor::SetInput(std::string varName, 
  std::string* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kString, (void*) varAddress, description};
  fInputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetInput(std::string varName, 
  std::vector<std::string>* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kStringVec, (void*) varAddress, description};
  fInputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetInput(std::string varName, 
  int* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kInt, (void*) varAddress, description};
  fInputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetInput(std::string varName, 
  std::vector<int>* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kIntVec, (void*) varAddress, description};
  fInputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetInput(std::string varName, 
  double* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kReal, (void*) varAddress, description};
  fInputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetInput(std::string varName, 
  std::vector<double>* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kRealVec, (void*) varAddress, description};
  fInputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetOutput(std::string varName, 
  std::string* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kString, (void*) varAddress, description};
  fOutputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetOutput(std::string varName, 
  std::vector<std::string>* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kStringVec, (void*) varAddress, description};
  fOutputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetOutput(std::string varName, 
  int* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kInt, (void*) varAddress, description};
  fOutputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetOutput(std::string varName, 
  std::vector<int>* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kIntVec, (void*) varAddress, description};
  fOutputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetOutput(std::string varName, 
  double* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kReal, (void*) varAddress, description};
  fOutputMap[varName] = input; 
}

void ORVOrcaRequestProcessor::SetOutput(std::string varName, 
  std::vector<double>* varAddress, std::string description)
{
  ORVOrcaReqInputOutput input = {kRealVec, (void*) varAddress, description};
  fOutputMap[varName] = input; 
}
