// orcaroot-header-sample.cc //
// Created by Sanshiro Enomoto on 1 March 2010. //


#include <iostream>
#include <iomanip>
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

    // optional: enable log message printing //
    //TOrcaRootHeader::SetGlobalLogStream(cerr);

    // must-do: read header from a file //
    TFile File(InputOrcaRootFileName);
    TOrcaRootHeader Header(File);



    // example: getting a string value, by basic key path navigation //
    string date = Header["Document Info"]["date"];
    cout << "date: " << date << endl;

    // example: getting an integer value, by basic key and index path navigation //
    int RunNumber = Header["ObjectInfo"]["DataChain"][1]["Run Control"]["RunNumber"];
    cout << "RunNumber: " << RunNumber << endl;

    // example: getting a real value, by a XPath-style string //
    float Intercept = Header["ObjectInfo/Crates[1]/Cards[1]/DaughterCards[1]/chan0/Intercept"];
    cout << "Intercept[1][1][1][0]: " << Intercept << endl;

    // example: getting a bool value, by mixed path //
    bool Enabled = Header["ObjectInfo/Crates[0]/Cards[1]"]["triggersEnabled[0]"];
    cout << "Enabled: " << Enabled << endl;

    // example: converting to a ROOT T(Obj)String //
    TObjString dataId = (const char*) Header["eventDescription/KatrinFLT/dataId"];
    dataId.Print(0);



    // advanced example: iterating known nodes //
    const TPlistNode& Crates = Header["ObjectInfo/Crates"];
    for (unsigned i = 0; i < Crates.Size(); i++) {

	const TPlistNode& Cards = Crates[i]["Cards"];
	for (unsigned j = 0; j < Cards.Size(); j++) {

	    cout << "filterGapSetting for crate " << i << " card " << j << ": ";
	    if (Cards[j]["filterGapSetting"].IsNull()) {
		cout << "UNDEFINED" << endl;
	    }
	    else {
		cout << Cards[j]["filterGapSetting"] << endl;
	    }
	}
    }

    // advanced example: iterating unknown nodes //
    const TPlistNode& DataDescription = Header["dataDescription"];
    for (unsigned i = 0; i < DataDescription.Size(); i++) {
	string ModelName = DataDescription.Keys()[i];

	const TPlistNode& DataElements = DataDescription[ModelName];
	for (unsigned j = 0; j < DataElements.Size(); j++) {
	    string DataName = DataElements.Keys()[j];

	    const TPlistNode& DataId = DataElements[DataName]["dataId"];
	    if (! DataId.IsNull()) {
		cout << hex << setfill('0') << setw(8) << (int) DataId << ": ";
		cout << ModelName << "." << DataName << endl;
	    }
	}
    }

    
    return 0;
}
