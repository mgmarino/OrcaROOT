// Plist.hh //
// Created by Sanshiro Enomoto on 1 April 2010. //


#ifndef Plsit_hh__
#define Plsit_hh__ 1

#include <string>
#include "PlistNode.hh"


class TPlist: public TPlistNode {
  public:
    TPlist(void);
    virtual ~TPlist();
    virtual void ParseXmlString(const std::string& PlistXmlString);
};


#endif
