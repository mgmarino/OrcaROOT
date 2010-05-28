// PlistParser.cc //
// Created by Sanshiro Enomoto on 1 March 2010. //


#include <string>
#include <iostream>
#include <deque>
#include <TSAXParser.h>
#include "PlistNode.hh"
#include "PlistParser.hh"

using namespace std;

#include <TList.h>
#include <TMap.h>
#include <TObjString.h>
#include <TString.h>

TPlistParser::TPlistParser(TPlistNode& RootNode)
 : _RootNode(RootNode)
{
}

TPlistParser::~TPlistParser()
{
}

void TPlistParser::OnStartElement(const char* Name, const TList* AttributeList)
{
    _CurrentText.erase();

    string TagName = Name;
    if (TagName == "plist") {
	//... TODO: check plist version here. It must be "1.0" ...//
	;
    }
    else if (TagName == "key") {
	;
    }
    else {
	StartValue(TagName);
    }
}

void TPlistParser::OnEndElement(const char* Name)
{
    string TagName = Name;
    if (TagName == "plist") {
	;
    }
    else if (TagName == "key") {
	_CurrentKey = _CurrentText;
    }
    else {
	EndValue(TagName);
	_CurrentKey.erase();
    }
}

void TPlistParser::OnCharacters(const char* Characters)
{
    _CurrentText += Characters;
}

void TPlistParser::OnWarning(const char* Text)
{
    _RootNode.LogStream(TPlistNode::Severity_Warning)
	<< "Waning: XML Parser: " << Text << endl;
}

Int_t TPlistParser::OnError(const char* Text)
{
    _RootNode.LogStream(TPlistNode::Severity_Error)
	<< "ERROR: XML Parser: " << Text << endl;
}

Int_t TPlistParser::OnFatalError(const char* Text)
{
    _RootNode.LogStream(TPlistNode::Severity_Fatal)
	<< "FATAL ERROR: XML Parser: " << Text << endl;
}

void TPlistParser::StartValue(const std::string& TypeName)
{
    if (_NodeStack.empty()) {
	_NodeStack.push_back(&_RootNode);
    }
    else {
	_NodeStack.push_back(_NodeStack.back()->CreateChildNode(_CurrentKey));
    }
}

void TPlistParser::EndValue(const std::string& TypeName)
{
    string CorrectedTypeName;
    if ((TypeName == "true") || (TypeName == "false")) {
	_CurrentText = TypeName;
	CorrectedTypeName = "bool";
    }
    else {
	CorrectedTypeName = TypeName;
    }

    _NodeStack.back()->SetValue(CorrectedTypeName, _CurrentText);
    _NodeStack.pop_back();
}
