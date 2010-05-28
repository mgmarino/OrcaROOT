// PlistNode.cc //
// Created by Sanshiro Enomoto on 1 March 2010. //
// Updated by Sanshiro Enomoto on 1 April 2010. //


#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "PlistNode.hh"

using namespace std;


static const char* g_NodeTypeNameTable[] = {
    /* this order must be same as "enum TNodeType" in PlistNode.hh */
    "null",
    "integer", "real", "bool",
    "string", "date",
    "data",
    "dict", "array"
};
static const int g_NumberOfTypes = (
    sizeof(g_NodeTypeNameTable) / sizeof(g_NodeTypeNameTable[0])
);

static const TPlistNode g_NullNode;
static map<string, int> g_NodeNameTypeTable;


TPlistNode::TPlistNode(void)
{
    _NodeType = NodeType_Null;
    _ParentNode = 0;

    _LogSeverity = TPlistNode::Severity_None;
    _LogStream = 0;
}

TPlistNode::TPlistNode(int NodeType)
{
    _NodeType = NodeType;
    _ParentNode = 0;

    _LogSeverity = TPlistNode::Severity_None;
    _LogStream = 0;

}

TPlistNode::TPlistNode(const TPlistNode& Node)
{
    // copying is prohibited //

    _NodeType = NodeType_Null;
    _ParentNode = 0;

    _LogSeverity = TPlistNode::Severity_None;
    _LogStream = 0;
}

TPlistNode& TPlistNode::operator=(const TPlistNode& Node)
{
    // copying is prohibited //

    _NodeType = NodeType_Null;
    _ParentNode = 0;

    _LogSeverity = TPlistNode::Severity_None;
    _LogStream = 0;

    return *this;
}

TPlistNode::~TPlistNode()
{
    for (unsigned i = 0; i < _ChildNodeList.size(); i++) {
	delete _ChildNodeList[i];
    }
}

const TPlistNode& TPlistNode::NullNode(void)
{
    return g_NullNode;
}

TPlistNode* TPlistNode::CreateChildNode(const std::string& Key)
{
    TPlistNode* ChildNode = new TPlistNode();
    ChildNode->_ParentNode = this;

    int Index = _ChildNodeList.size();
    _ChildNodeList.push_back(ChildNode);

    if (! Key.empty()) {
	_ChildNodeKeyList.push_back(Key);
	_ChildNodeIndexTable[Key] = Index;
    }

    return ChildNode;
}

void TPlistNode::SetValue(const std::string& TypeName, const std::string& Value)
{
    if (g_NodeNameTypeTable.empty()) {
	for (int i = 0; i < g_NumberOfTypes; i++) {
	    g_NodeNameTypeTable[g_NodeTypeNameTable[i]] = i;
	}
    }

    _NodeType = TPlistNode::NodeType_Null;
    if (g_NodeNameTypeTable.count(TypeName)) {
	_NodeType = g_NodeNameTypeTable[TypeName];
    }

    if (_NodeType == NodeType_Null) {
	LogStream(Severity_Error)
	    << "unknown node type: [" << TypeName << "]: "
	    << " at \"" << NodePath() << "\"" << endl;
    }
    else if (_NodeType == NodeType_Dictionary) {
	if (_ChildNodeList.size() != _ChildNodeKeyList.size()) {
	    LogStream(Severity_Error) 
		<< "ERROR: no key for dict element: " 
		<< " at \"" << NodePath() << "\"" << endl;
	}
    }

    if (
	(_NodeType != TPlistNode::NodeType_Dictionary) &&
	(_NodeType != TPlistNode::NodeType_Array)
    ){
	if (!_ChildNodeList.empty()) {
	    LogStream(Severity_Error) 
		<< "ERROR: child node insertion to non-collection node: " 
		<< " at \"" << NodePath() << "\"" << endl;
	}
	_Value = Value;
    }
}

const TPlistNode& TPlistNode::ChildNodeOf(const std::string& Key) const
{
    std::map<std::string, int>::const_iterator i = _ChildNodeIndexTable.find(Key);
    if (i != _ChildNodeIndexTable.end()) {
	return *_ChildNodeList[i->second];
    }

    if (Key.empty()) {
	return *this;
    }
    else if (Key[0] == '/') {
	return ChildNodeOf(Key.substr(1));
    }

    if (Key[0] == '[') {
	int End = Key.find_first_of(']');
	if (End == string::npos) {
	    LogStream(Severity_Error) 
		<< "ERROR: [] mismatch: "
		<< "\"" << Key << "\" at \"" << NodePath() << "\"" << endl;
	    return NullNode();
	}
	int Index;
	istringstream is(Key.substr(1, End-1));
	if (is >> Index) {
	    string Excess;
	    if (is >> Excess) {
		LogStream(Severity_Error) 
		    << "ERROR: non-integer array index: "
		    << "\"[" << Key.substr(1, End-1) << "]\""
		    << " at \"" << NodePath() << "\"" << endl;
		return NullNode();
	    }
	    return this->operator[](Index).ChildNodeOf(Key.substr(End+1));
	}
	else {
	    LogStream(Severity_Error) 
		<< "ERROR: non-integer array index: "
		<< "\"[" << Key.substr(1, End-1) << "]\""
		<< " at \"" << NodePath() << "\"" << endl;
	    return NullNode();
	}
    }

    int End = Key.find_first_of("/[");
    if (End == string::npos) {
	LogStream(Severity_Warning) 
	    << "Warning: node undefined: "
	    << "\"" << Key << "\" at \"" << NodePath() << "\"" << endl;
	return NullNode();
    }

    return ChildNodeOf(Key.substr(0, End)).ChildNodeOf(Key.substr(End));
}

std::string TPlistNode::NodePathOf(const TPlistNode* ChildNode) const
{
    string NodeName;
    for (unsigned i = 0; i < _ChildNodeList.size(); i++) {
	if (_ChildNodeList[i] == ChildNode) {
	    if (_NodeType == NodeType_Dictionary) {
		NodeName = "/" + _ChildNodeKeyList[i];
	    }
	    else if (_NodeType == NodeType_Array) {
		ostringstream ArrayIndex;
		ArrayIndex << "[" << i << "]";
		NodeName = ArrayIndex.str();
	    }
	}
    }

    if (! NodeName.empty() && _ParentNode) {
	NodeName = _ParentNode->NodePathOf(this) + NodeName;
    }

    return NodeName;
}

void TPlistNode::Dump(std::ostream& os, const std::string& CurrentPath)
{
    if (! _Value.empty()) {
	os << CurrentPath << ": ";
	os << "(" << g_NodeTypeNameTable[_NodeType] << ") ";
	os << _Value << endl;
    }

    for (unsigned i = 0; i < _ChildNodeList.size(); i++) {
	if (_NodeType == NodeType_Dictionary) {
	    string Key = _ChildNodeKeyList[i];
	    _ChildNodeList[i]->Dump(os, CurrentPath + "/" + Key);
	}
	else {
	    ostringstream ArrayPath;
	    ArrayPath << CurrentPath << "[" << i << "]";
	    _ChildNodeList[i]->Dump(os, ArrayPath.str());
	}
    }
}



class TNullStreamBuffer: public std::streambuf {
  protected:
    virtual int overflow(int ch) { return ch; }
};

class TNullStream: public std::ostream {
  public:
    TNullStream(void): ostream(&_StreamBuffer) {}
  private:
    TNullStreamBuffer _StreamBuffer;
};

static TNullStream g_NullStream;
ostream* TPlistNode::_GlobalLogStream = 0;
int TPlistNode::_GlobalLogSeverity = Severity_None;


void TPlistNode::SetGlobalLogStream(std::ostream& Stream, int Severity)
{
    _GlobalLogStream = &Stream;
    _GlobalLogSeverity = Severity;
}

void TPlistNode::SetLogStream(std::ostream& Stream, int Severity) const
{
    _LogStream = &Stream;
    _LogSeverity = Severity;
}

std::ostream& TPlistNode::LogStream(int Severity) const
{
    if (_LogStream) {
	if (Severity >= _LogSeverity) {
	    return *_LogStream;
	}
    }
    else if (_ParentNode) {
	return _ParentNode->LogStream(Severity);
    }
    else if (_GlobalLogStream) {
	if (Severity >= _GlobalLogSeverity) {
	    return *_GlobalLogStream;
	}
    }
    
    return g_NullStream;
}
