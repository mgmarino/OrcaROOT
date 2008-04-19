// ORXmlPlist.cc

#include "ORXmlPlist.hh"
#include "TXMLNode.h"
#include <fstream>
#include <cstdlib>
#include "ORLogger.hh"
#include "TDOMParser.h"

ORXmlPlist::ORXmlPlist(const char* fullHeaderAsString, size_t lengthOfBuffer)
{ 
  fDictionary = NULL; 

  if (fullHeaderAsString) LoadXmlPlist(fullHeaderAsString, lengthOfBuffer);
}

ORXmlPlist::~ORXmlPlist() 
{ 
  if (fDictionary != NULL) delete fDictionary;
}

bool ORXmlPlist::LoadXmlPlistFromFile(const char* fileName)
{
  // We must use a local buffer but then we must handle the memory of the
  // buffer.
  // We assume that we get JUST the header.
  std::ifstream headerFile(fileName);
  if(!headerFile.good()) {
    ORLog(kError) << "Error opening file: " << fileName << std::endl;
    return false;
  }
  fRawXML.ReadFile(headerFile);
  return LoadXmlPlist(fRawXML.Data(), fRawXML.Length());
}

bool ORXmlPlist::LoadXmlPlist(const char* fullHeaderAsString, size_t lengthOfBuffer)
{
  if(ORLogger::GetSeverity() <= ORLogger::kDebug)
  {
    // dump the header out for debugging purposes.
    std::ofstream headerFile("tmp_header.xml");
    if (!headerFile.good()) {
      ORLog(kError) << "LoadXmlPlist(): You are running in debug mode. " 
                    << "Problem opening file tmp_header.xml. "
                    << "Do you have write permissions to the directory from "
                    << "which you are running your application?" << std::endl;
      return false;
    }
    headerFile.write(fullHeaderAsString, lengthOfBuffer);
    headerFile.close();
  }

  ORLog(kDebug) << "LoadXmlPlist(): Parsing..." << std::endl;
  TDOMParser domParser;
  domParser.ParseBuffer(fullHeaderAsString, lengthOfBuffer);
  TXMLDocument* doc = domParser.GetXMLDocument();
  if(((size_t)fRawXML.Length()) != lengthOfBuffer) { 
    //we have to copy to fRawXML.  Making sure we're not copying again.
    fRawXML.Resize(lengthOfBuffer);
  }
  fRawXML.Replace(0, lengthOfBuffer, fullHeaderAsString, lengthOfBuffer);
  if (doc == NULL) {
    ORLog(kError) << "LoadXmlPlist(): couldn't parse buffer " 
      << std::endl;
    return false;
  }

  ORLog(kDebug) << "LoadXmlPlist(): Getting root node..." << std::endl;
  TXMLNode* rootNode = doc->GetRootNode();
  if (rootNode == NULL) {
    ORLog(kError) << "LoadXmlPlist(): root node was NULL in buffer " 
                  << std::endl;
    return false;
  }
  if(std::string(rootNode->GetNodeName()) != "plist") {
    ORLog(kError) << "LoadXmlPlist(): root node was not a plist in buffer"
                  << std::endl;
    return false;
  }

  ORLog(kDebug) << "LoadXmlPlist(): Getting root dictionary..." << std::endl;
  TXMLNode* rootDict = FindChildByName("dict", rootNode);
  if (rootDict == NULL) {
    ORLog(kError) << "LoadXmlPlist(): couldn't find root dictionary in buffer" 
                  << std::endl;
    return false;
  }

  ORLog(kDebug) << "LoadXmlPlist(): Loading root dictionary..." << std::endl;
  if (fDictionary != NULL) delete fDictionary;
  fDictionary = new ORDictionary("rootDict"); // deleted in deconstructor
  return LoadDictionary(rootDict, fDictionary); 
}

const ORVDictValue* ORXmlPlist::LookUp(std::string key) const 
{ 
  if(fDictionary == NULL) {
    ORLog(kError) << "LookUp(): dictionary not loaded" << std::endl;
    return 0;
  }
  return fDictionary->LookUp(key); 
}

bool ORXmlPlist::LoadDictionary(TXMLNode* dictNode, ORDictionary* dictionary)
{
  ORLog(kDebug) << "LoadDictionary(): Loading dictionary " 
                << dictionary->GetName() << "..." << std::endl;
  TXMLNode* child = dictNode->GetChildren();
  while (child != NULL && child->HasNextNode()) {
    while (child->HasNextNode() && std::string(child->GetNodeName()) != "key") {
      child = child->GetNextNode();
    }
    if (!child->HasNextNode()) break;
    std::string keyname = child->GetText();
    child = child->GetNextNode();
    if (!child->HasNextNode()) break;

    while (child->HasNextNode() && std::string(child->GetNodeName()) == "text") {
      child = child->GetNextNode();
    }
    std::string valtype = child->GetNodeName();
    if(ORLogger::GetSeverity() <= ORLogger::kDebug) {
      ORLog(kDebug) << "LoadDictionary(): Handling " << valtype << " " << keyname << std::endl;
    }
    ORVDictValue* dictValue = NULL;
    if (valtype == "dict") {
      dictValue = new ORDictionary(keyname);
      LoadDictionary(child, (ORDictionary*) dictValue); 
    } else if (valtype == "string") {
      const char* str = child->GetText();
      if(str == NULL) dictValue = new ORDictValueS("");
      else dictValue = new ORDictValueS(str);
    } else if (valtype == "real") {
      dictValue = new ORDictValueR(atof(child->GetText()));
    } else if (valtype == "integer") {
      dictValue = new ORDictValueI(atoi(child->GetText()));
    } else if (valtype == "false") {
      dictValue = new ORDictValueB(false);
    } else if (valtype == "true") {
      dictValue = new ORDictValueB(true);
    } else if (valtype == "array") {
      dictValue = new ORDictValueA(keyname);
      LoadArray(child, (ORDictValueA*) dictValue);
    } else {
      ORLog(kWarning) << "LoadDictionary(): unsupported value type " << valtype << " with name " << keyname << std::endl;
    }
    dictionary->LoadEntry(keyname, dictValue);
    ORLog(kDebug) << "LoadDictionary(): loaded entry " << keyname << std::endl;
    if (keyname == "dataId") {
      ORLog(kDebug) << dictionary->GetName() + "::LoadDictionary(): dataId = " 
                    << ((ORDictValueI*) dictValue)->GetI() << std::endl;
    }
    child = child->GetNextNode();
  }
  ORLog(kDebug) << "LoadDictionary(): Finished loading dictionary " 
                << dictionary->GetName() << "..." << std::endl;
  return true;
}

bool ORXmlPlist::LoadArray(TXMLNode* arrayNode, ORDictValueA* dictValueA)
{
  ORLog(kDebug) << "LoadArray(): Loading an array " << std::endl;
  TXMLNode* child = arrayNode->GetChildren();
  while (child != NULL && child->HasNextNode()) {
    while (child->HasNextNode() && std::string(child->GetNodeName()) == "text") {
      child = child->GetNextNode();
    }
    if (!child->HasNextNode()) break;
    std::string valtype = child->GetNodeName();
    ORLog(kDebug) << "LoadArray(): Handling " << valtype << std::endl;
    ORVDictValue* dictValue = NULL;
    if (valtype == "dict") {
      dictValue = new ORDictionary;
      LoadDictionary(child, (ORDictionary*) dictValue); 
    }
    else if (valtype == "string") {
      const char* str = child->GetText();
      if(str == NULL) dictValue = new ORDictValueS("");
      else dictValue = new ORDictValueS(str);
    } else if (valtype == "real") {
      dictValue = new ORDictValueR(atof(child->GetText()));
    } else if (valtype == "integer") {
      dictValue = new ORDictValueI(atoi(child->GetText()));
    } else if (valtype == "false") {
      dictValue = new ORDictValueB(false);
    } else if (valtype == "true") {
      dictValue = new ORDictValueB(true);
    } else {
      ORLog(kWarning) << "LoadArray(): unsupported value type " << valtype << std::endl;
    }
    dictValueA->LoadValue(dictValue);
    child = child->GetNextNode();
  }
  ORLog(kDebug) << "LoadArray(): Finished loading array " << std::endl;
  return true;
}

TXMLNode* ORXmlPlist::FindChildByName(const char* name, TXMLNode* parent)
{
  if (name == NULL || parent == NULL) return NULL;

  TXMLNode* child = parent->GetChildren();
  while (child != NULL && child->HasNextNode()) {
    ORLog(kDebug) << "FindChildByName(): examining child " << child->GetNodeName() << std::endl;
    if (std::string(child->GetNodeName()) == name) return child;
    child = child->GetNextNode();
  }

  ORLog(kWarning) << "FindChildByName(): couldn't find child '" << name 
                  << "' under parent '" << parent->GetNodeName() << "'." << std::endl;

  return NULL;
}


