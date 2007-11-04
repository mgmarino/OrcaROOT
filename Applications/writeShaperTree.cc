#include <iostream>
#include <fstream>
#include "TFile.h"
#include "ORDataProcManager.hh"
#include "ORFileReader.hh"
#include "ORLogger.hh"
#include "ORShaperShaperTreeWriter.hh"
#include "ORHistWriter.hh"

using namespace std;


int main()
{
  ORLogger::SetSeverity(ORLogger::kRoutine);

  ORLog(kRoutine) << "Loading data..." << endl;
  ORFileReader orcaFileReader("run194ecpu");
  //ORSocketReader orcaFileReader("128.95.100.213", 44666);
  if (!orcaFileReader.OKToRead()) return 1;

  ORLog(kRoutine) << "Setting up data processing manager..." << endl;
  ORDataProcManager dataProcManager(&orcaFileReader);

  ORLog(kRoutine) << "Opening output file..." << endl;
  TFile* outFile = TFile::Open("run194ShaperData.root", "RECREATE");

  ORLog(kRoutine) << "Building shaperShaperTreeWriter..." << endl;
  ORShaperShaperTreeWriter shaperShaperTreeWriter;
  ORLog(kRoutine) << "Adding shaperShaperTreeWriter..." << endl;
  dataProcManager.AddProcessor(&shaperShaperTreeWriter);

  ORLog(kRoutine) << "Building shaperHistWriter..." << endl;
  ORShaperShaperDecoder shaperShaperDecoder;
  ORHistWriter shaperHistWriter(&shaperShaperDecoder);
  ORLog(kRoutine) << "Adding shaperHistWriter..." << endl;
  dataProcManager.AddProcessor(&shaperHistWriter);

  ORLog(kRoutine) << "Start processing..." << endl;
  dataProcManager.ProcessDataStream();

  ORLog(kRoutine) << "Closing output file..." << endl;
  outFile->Close();

  return 0;
}

