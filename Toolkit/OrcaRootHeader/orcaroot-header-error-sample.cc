// orcaroot-header-error-sample.cc //
// Updated by Sanshiro Enomoto on 1 April 2010. //


#include <iostream>
#include <TFile.h>

#include "OrcaRootHeader.hh"

using namespace std;


int main(int argc, char** argv)
{
    if (argc < 2) {
	cerr << "Usage: " << argv[0] << " ORCA_ROOT_FILE" << endl;
	return -1;
    }
    const char* InputOrcaRootFileName = argv[1];

    // optional: enable log message printing //
    TOrcaRootHeader::SetGlobalLogStream(cerr);

    // must-do: read header from a file //
    TFile File(InputOrcaRootFileName);
    TOrcaRootHeader Header(File);

    // check header parse errors //
    if (Header.IsNull()) {
	cerr << "ERROR: unable to read ORCA header" << endl;
	return -1;
    }


    string WrongNodePath = "Document Info/Date";

    // example: simple error check //
    if (Header[WrongNodePath].IsNull()) {
	cerr << "ERROR: unable to find node: " << WrongNodePath << endl;
    }
    
    // example: simple error message printing //
    Header.SetLogStream(cerr);
    string Value = Header[WrongNodePath];

    // note: SetLogStream() override the global setting if enabled.
    // note: you can SetLogStream() for any part of node branches as well.
    // example: Header["Document Info"].SetLogStream(cerr);

    // note: optional second parameter specifies minimum severity to print.
    // severities: Debug, Trace, Routine, Warning, Error, Fatal, None
    // note: default severity is Severity_Warning.
    // note: setting Severity_None prohibits all message printing.
    // example: Header.SetLogStream(cerr, TPlistNode::Severity_None);



    // advanced example: throwing an exception on error //
    Header.SetLogStream(cerr, TPlistNode::Severity_None);
    try {
	string Exception = "Unable to find node";   // can be of any type
	string Value = Header[WrongNodePath].ValidateOrThrow(Exception);
    }
    catch (string Exception) {
	cerr << "ERROR: " << Exception << endl;
    }

    // advanced example: combination of messaging and exception //
    ostringstream MyErrorStream;
    Header.SetLogStream(MyErrorStream); 
    try {
	int ErrorCode = __LINE__;
	string Value = Header[WrongNodePath].ValidateOrThrow(ErrorCode);
    }
    catch (int ErrorCode) {
	cerr << "ERROR: " << ErrorCode << ": " << MyErrorStream.str() << endl;
    }
    

    return 0;
}
