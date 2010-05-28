// PlistParser.hh //
// Created by Sanshiro Enomoto on 1 March 2010. //


#ifndef PlistParser_hh__
#define PlistParser_hh__ 1

#include <string>
#include <deque>
#include <TSAXParser.h>
#include "PlistNode.hh"


class TPlistParser: public TSAXParser {
  public:
    TPlistParser(TPlistNode& RootNode);
    virtual ~TPlistParser();
    virtual void OnStartElement(const char* Name, const TList* AttributeList);
    virtual void OnEndElement(const char* Name);
    virtual void OnCharacters(const char* Characters);
    virtual void OnWarning(const char* Text);
    virtual Int_t OnError(const char* Text);
    virtual Int_t OnFatalError(const char* Text);
  protected:
    virtual void StartValue(const std::string& TypeName);
    virtual void EndValue(const std::string& TypeName);
  protected:
    TPlistNode& _RootNode;
    std::string _CurrentKey, _CurrentText;
  private:
    std::deque<TPlistNode*> _NodeStack;
};


#endif
