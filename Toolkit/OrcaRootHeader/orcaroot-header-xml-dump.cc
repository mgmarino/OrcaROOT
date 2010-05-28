// orcaroot-header-dump.cc //
// Updated by Sanshiro Enomoto on 1 April 2010. //


#include <iostream>
#include <TFile.h>
#include <TObjString.h>

#include "OrcaRootHeader.hh"

using namespace std;


int main(int argc, char** argv)
{
    if (argc < 2) {
	cerr << "Usage: " << argv[0] << " ORCA_ROOT_FILE" << endl;
	return -1;
    }
    const char* InputOrcaRootFileName = argv[1];

    TFile File(InputOrcaRootFileName);
    TObjString* HeaderString = TOrcaRootHeader::GetHeaderObjStringFromFile(File);
    cout << HeaderString->GetString().Data() << endl;

    return 0;
}
