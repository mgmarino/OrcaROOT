// PlistNode.hh //
// Created by Sanshiro Enomoto on 1 March 2010. //
// Updated by Sanshiro Enomoto on 1 April 2010. //


#ifndef PlsitNode_hh__
#define PlsitNode_hh__ 1

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <typeinfo>


class TPlistNode {
  public:
    TPlistNode(void);
    explicit TPlistNode(int NodeType);
    virtual ~TPlistNode(void);
  private:
    // copying is not allowed //
    TPlistNode(const TPlistNode& Node);
    TPlistNode& operator=(const TPlistNode& Node);
  public:
    // for user iteration //
    inline bool IsNull(void) const;
    inline int NodeType(void) const;
    inline size_t Size(void) const;
    inline const std::vector<std::string>& Keys(void) const;
    template<typename T> inline operator T() const;
    inline const TPlistNode& operator[](const std::string& Key) const;
    inline const TPlistNode& operator[](int Index) const;
    inline std::string NodePath(void) const;
    virtual void Dump(std::ostream& os, const std::string& CurrentPath = "");
  public:
    // for node tree construction //
    virtual TPlistNode* CreateChildNode(const std::string& Key = "");
    virtual void SetValue(const std::string& TypeName, const std::string& Value);
  public:
    // for log/error messaging //
    static void SetGlobalLogStream(std::ostream& Stream, int Severity = Severity_Warning);
    void SetLogStream(std::ostream& Stream, int Severity = Severity_Warning) const;
    std::ostream& LogStream(int Severity) const;
    template<typename T> inline const TPlistNode& ValidateOrThrow(const T& Exception) const throw(T);
  private:
    // for internal use //
    std::string NodePathOf(const TPlistNode* ChildNode) const;
    const TPlistNode& ChildNodeOf(const std::string& Key) const;
    static const TPlistNode& NullNode(void);
  public:
    enum TNodeType {
 	NodeType_Null,
	NodeType_Integer,
	NodeType_Real,
	NodeType_Bool,
	NodeType_String,
	NodeType_Date,
	NodeType_Data,
	NodeType_Dictionary,
	NodeType_Array,
	_NumberOfNodeTypes
    };
    enum TLogSeverity {
	Severity_Debug = 0,
	Severity_Trace = 1,
	Severity_Routine = 2,
	Severity_Warning = 3,
	Severity_Error = 4,
	Severity_Fatal = 5,
	Severity_None = 6
    };
  private:
    int _NodeType;
    std::string _Value;
    TPlistNode* _ParentNode;
    std::vector<TPlistNode*> _ChildNodeList;
    std::vector<std::string> _ChildNodeKeyList;
    std::map<std::string, int> _ChildNodeIndexTable;
  private:
    mutable std::ostream* _LogStream;
    mutable int _LogSeverity;
    static std::ostream* _GlobalLogStream;
    static int _GlobalLogSeverity;
};



inline bool TPlistNode::IsNull(void) const
{
    return _NodeType == NodeType_Null;
}

inline int TPlistNode::NodeType(void) const
{
    return _NodeType;
}

inline std::string TPlistNode::NodePath(void) const
{
    if (_ParentNode) {
	return _ParentNode->NodePathOf(this);
    }
    else {
	return (_NodeType != NodeType_Null) ? "/" : "(null)";
    }
}

inline size_t TPlistNode::Size(void) const
{
    return _ChildNodeList.size();
}

inline const std::vector<std::string>& TPlistNode::Keys(void) const
{
    return _ChildNodeKeyList;
}

template<typename T> inline TPlistNode::operator T() const
{
    // T-type value must be able to be initialized with 0, and
    // T-type value must be able to be ostream'ed.
    // Otherwise explicit template instantiation is necessary.

    if (_NodeType == NodeType_Null) {
	return T(0);
    }

    T Value;

    std::istringstream is(_Value);
    if (! (is >> Value)) {
	LogStream(Severity_Error) 
	    << "ERROR: unable to convert to " << typeid(T).name()
	    << ": \"" << _Value << "\" at \"" << NodePath() << "\""
	    << std::endl;
    }

    std::string Excess;
    if (is >> Excess) {
	LogStream(Severity_Error) 
	    << "ERROR: unable to convert to " << typeid(T).name()
	    << ": \"" << _Value << "\" at \"" << NodePath() << "\""
	    << std::endl;
    }

    return Value;
}

template<> inline TPlistNode::operator bool() const
{
    return (_Value == "true");
}

template<> inline TPlistNode::operator std::string() const
{
    return _Value;
}

template<> inline TPlistNode::operator const char*() const
{
    return _Value.c_str();
}

template<> inline TPlistNode::operator char*() const
{
    return const_cast<char*>(_Value.c_str());
}

inline const TPlistNode& TPlistNode::operator[](const std::string& Key) const
{
    std::map<std::string, int>::const_iterator i = _ChildNodeIndexTable.find(Key);
    if (i == _ChildNodeIndexTable.end()) {
	return ChildNodeOf(Key);
    }
    else {
	return *_ChildNodeList[i->second];
    }
}

inline const TPlistNode& TPlistNode::operator[](int Index) const
{
    if ((Index < 0) || (Index >= _ChildNodeList.size())) {
	LogStream(Severity_Warning) 
	    << "Warning: node undefined: " 
	    << "[" << Index << "]" << " at \"" << NodePath() << "\""
	    << std::endl;
	return NullNode();
    }
    else {
	return *_ChildNodeList[Index];
    }
}

template<typename T> inline const TPlistNode& TPlistNode::ValidateOrThrow(const T& Exception) const throw(T)
{
    if (IsNull()) {
	throw Exception;
    }
    return *this;
}

inline std::ostream& operator<<(std::ostream& os, const TPlistNode& PlistNode) 
{
    std::string Value = PlistNode;
    return os << Value;
}

#endif
