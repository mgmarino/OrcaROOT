// Plist.cc //
// Created by Sanshiro Enomoto on 1 April 2010. //


#include <string>
#include "PlistNode.hh"
#include "PlistParser.hh"
#include "Plist.hh"

using namespace std;


TPlist::TPlist(void)
{
}

TPlist::~TPlist()
{
}

void TPlist::ParseXmlString(const string& PlistXmlString)
{
    TPlistParser(*this).ParseBuffer(
	PlistXmlString.c_str(), PlistXmlString.size()
    );
}
