// ORXmlPlist.cc

#include "ORXmlPlist.hh"

#include <fstream>
#include "ORLogger.hh"
#include "TDOMParser.h"

using namespace std;

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
  ifstream headerFile(fileName);
  if(!headerFile.good()) {
    ORLog(kError) << "Error opening file: " << fileName << endl;
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
    ofstream headerFile("tmp_header.xml");
    if (!headerFile.good()) {
      ORLog(kError) << "LoadXmlPlist(): You are running in debug mode. " 
                    << "Problem opening file tmp_header.xml. "
                    << "Do you have write permissions to the directory from "
                    << "which you are running your application?" << endl;
      return false;
    }
    headerFile.write(fullHeaderAsString, lengthOfBuffer);
    headerFile.close();
  }

  ORLog(kDebug) << "LoadXmlPlist(): Parsing..." << endl;
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
      << endl;
    return false;
  }

  ORLog(kDebug) << "LoadXmlPlist(): Getting root node..." << endl;
  TXMLNode* rootNode = doc->GetRootNode();
  if (rootNode == NULL) {
    ORLog(kError) << "LoadXmlPlist(): root node was NULL in buffer " 
                  << endl;
    return false;
  }
  if(string(rootNode->GetNodeName()) != "plist") {
    ORLog(kError) << "LoadXmlPlist(): root node was not a plist in buffer"
                  << endl;
    return false;
  }

  ORLog(kDebug) << "LoadXmlPlist(): Getting root dictionary..." << endl;
  TXMLNode* rootDict = FindChildByName("dict", rootNode);
  if (rootDict == NULL) {
    ORLog(kError) << "LoadXmlPlist(): couldn't find root dictionary in buffer" 
                  << endl;
    return false;
  }

  ORLog(kDebug) << "LoadXmlPlist(): Loading root dictionary..." << endl;
  if (fDictionary != NULL) delete fDictionary;
  fDictionary = new ORDictionary("rootDict"); // deleted in deconstructor
  return LoadDictionary(rootDict, fDictionary); 
}

const ORVDictValue* ORXmlPlist::LookUp(std::string key) const 
{ 
  if(fDictionary == NULL) {
    ORLog(kError) << "LookUp(): dictionary not loaded" << endl;
    return 0;
  }
  return fDictionary->LookUp(key); 
}

bool ORXmlPlist::LoadDictionary(TXMLNode* dictNode, ORDictionary* dictionary)
{
  ORLog(kDebug) << "LoadDictionary(): Loading dictionary " 
                << dictionary->GetName() << "..." << endl;
  TXMLNode* child = dictNode->GetChildren();
  while (child != NULL && child->HasNextNode()) {
    while (child->HasNextNode() && string(child->GetNodeName()) != "key") {
      child = child->GetNextNode();
    }
    if (!child->HasNextNode()) break;
    string keyname = child->GetText();
    child = child->GetNextNode();
    if (!child->HasNextNode()) break;

    while (child->HasNextNode() && string(child->GetNodeName()) == "text") {
      child = child->GetNextNode();
    }
    string valtype = child->GetNodeName();
    if(ORLogger::GetSeverity() <= ORLogger::kDebug) {
      ORLog(kDebug) << "LoadDictionary(): Handling " << valtype << " " << keyname << endl;
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
      ORLog(kWarning) << "LoadDictionary(): unsupported value type " << valtype << " with name " << keyname << endl;
    }
    dictionary->LoadEntry(keyname, dictValue);
    ORLog(kDebug) << "LoadDictionary(): loaded entry " << keyname << endl;
    if (keyname == "dataId") {
      ORLog(kDebug) << dictionary->GetName() + "::LoadDictionary(): dataId = " 
                    << ((ORDictValueI*) dictValue)->GetI() << endl;
    }
    child = child->GetNextNode();
  }
  ORLog(kDebug) << "LoadDictionary(): Finished loading dictionary " 
                << dictionary->GetName() << "..." << endl;
  return true;
}

bool ORXmlPlist::LoadArray(TXMLNode* arrayNode, ORDictValueA* dictValueA)
{
  ORLog(kDebug) << "LoadArray(): Loading an array " << endl;
  TXMLNode* child = arrayNode->GetChildren();
  while (child != NULL && child->HasNextNode()) {
    while (child->HasNextNode() && string(child->GetNodeName()) == "text") {
      child = child->GetNextNode();
    }
    if (!child->HasNextNode()) break;
    string valtype = child->GetNodeName();
    ORLog(kDebug) << "LoadArray(): Handling " << valtype << endl;
    ORVDictValue* dictValue = NULL;
    if (valtype == "dict") {
      dictValue = new ORDictionary;
      LoadDictionary(child, (ORDictionary*) dictValue); 
    }
    else if (valtype == "string") {
      dictValue = new ORDictValueS(child->GetText());
    } else if (valtype == "real") {
      dictValue = new ORDictValueR(atof(child->GetText()));
    } else if (valtype == "integer") {
      dictValue = new ORDictValueI(atoi(child->GetText()));
    } else if (valtype == "false") {
      dictValue = new ORDictValueB(false);
    } else if (valtype == "true") {
      dictValue = new ORDictValueB(true);
    } else {
      ORLog(kWarning) << "LoadArray(): unsupported value type " << valtype << endl;
    }
    dictValueA->LoadValue(dictValue);
    child = child->GetNextNode();
  }
  ORLog(kDebug) << "LoadArray(): Finished loading array " << endl;
  return true;
}

TXMLNode* ORXmlPlist::FindChildByName(const char* name, TXMLNode* parent)
{
  if (name == NULL || parent == NULL) return NULL;

  TXMLNode* child = parent->GetChildren();
  while (child != NULL && child->HasNextNode()) {
    ORLog(kDebug) << "FindChildByName(): examining child " << child->GetNodeName() << endl;
    if (string(child->GetNodeName()) == name) return child;
    child = child->GetNextNode();
  }

  ORLog(kWarning) << "FindChildByName(): couldn't find child '" << name 
                  << "' under parent '" << parent->GetNodeName() << "'." << endl;

  return NULL;
}


