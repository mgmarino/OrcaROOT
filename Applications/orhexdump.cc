#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include "ORDataProcManager.hh"
#include "ORFileReader.hh"
#include "ORLogger.hh"
#include "ORSocketReader.hh"

#include "OR1DHistoHistogramsDecoder.hh"
#include "ORAD3511ADCDecoder.hh"
#include "ORAD413ADCDecoder.hh"
#include "ORAD811ADCDecoder.hh"
#include "ORADC2249ADCDecoder.hh"
#include "ORAugerFLTEnergyDecoder.hh"
#include "ORCaen775tdcDecoder.hh"
#include "ORCaen785adcDecoder.hh"
#include "ORCaen792qdcDecoder.hh"
#include "ORDGF4cEventDecoder.hh"
#include "ORDGF4cMCADecoder.hh"
#include "ORL2551ScalersDecoder.hh"
#include "ORL4532channelTriggerDecoder.hh"
#include "ORL4532mainTriggerDecoder.hh"
#include "ORLC950ScopeDataDecoder.hh"
#include "ORRunDecoder.hh"
#include "ORShaperShaperDecoder.hh"
#include "ORTDC3377tdcDecoder.hh"
#include "ORTek754DScopeDataDecoder.hh"
#include "ORTrigger32ClockDecoder.hh"
#include "ORTrigger32LiveTimeDecoder.hh"
#include "ORVTrigger32GTIDDecoder.hh"

using namespace std;

static const char Usage[] =
"\n"
"\n"
"Usage: orhexdump [options] [input file(s) / socket host:port]\n"
"\n"
"The one required argument is either the name of a file to be processed or\n"
"a host and port of a socket from which to read data. For a file, you may\n"
"enter a series of files to be processed, or use a wildcard like \"file*.dat\"\n"
"For a socket, the argument should be formatted as host:port.\n"
"\n"
"Available options:\n"
"  --help : print this message and exit\n"
"  --verbosity [verbosity] : set the severity/verbosity for the logger.\n"
"    Choices are: debug, trace, routine, warning, error, and fatal.\n"
"\n"
"Example usage:\n"
"orhexdump run194ecpu\n"
"  Hex-dump local file run194ecpu with default verbosity, etc.\n"
"orhexdump run*\n"
"  Same as before, but run over all files beginning with \"run\".\n"
"orhexdump --verbosity debug run194ecpu\n"
"  The same, but with example usage of the verbosity option.\n"
"  Lots of debugging output will appear.\n"
"orhexdump 128.95.100.213:44666\n"
"  Hex dump orca stream on host 128.95.100.213, port 44666 with default verbosity, etc.\n"
"\n"
"\n";


int main(int argc, char** argv)
{
  if(argc == 1) {
    ORLog(kError) << "You must supply some options" << endl << Usage;
    return 1;
  }


  static struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"verbosity", required_argument, 0, 'v'},
  };

  string label = "OR";
  ORVReader* reader = NULL;

  while(1) {
    char optId = getopt_long(argc, argv, "", longOptions, NULL);
    if(optId == -1) break;
    switch(optId) {
      case('h'): // help
        cout << Usage;
        return 0;
      case('v'): // verbosity
        if(strcmp(optarg, "debug") == 0) ORLogger::SetSeverity(ORLogger::kDebug);
        else if(strcmp(optarg, "trace") == 0) ORLogger::SetSeverity(ORLogger::kTrace);
        else if(strcmp(optarg, "routine") == 0) ORLogger::SetSeverity(ORLogger::kRoutine);
        else if(strcmp(optarg, "warning") == 0) ORLogger::SetSeverity(ORLogger::kWarning);
        else if(strcmp(optarg, "error") == 0) ORLogger::SetSeverity(ORLogger::kError);
        else if(strcmp(optarg, "fatal") == 0) ORLogger::SetSeverity(ORLogger::kFatal);
        else {
          ORLog(kWarning) << "Unknown verbosity setting " << optarg 
                          << "; using kRoutine" << endl;
          ORLogger::SetSeverity(ORLogger::kRoutine);
        }
        break;
      default: // unrecognized option
        ORLog(kError) << Usage;
        return 1;
    }
  }

  if (argc <= optind) {
    ORLog(kError) << "You must supply a filename or socket host:port" << endl
                  << Usage << endl;
    return 1;
  }

  string readerArg = argv[optind];
  size_t iColon = readerArg.find(":");
  if (iColon == string::npos) {
    reader = new ORFileReader;
    for (int i=optind; i<argc; i++) {
      ((ORFileReader*) reader)->AddFileToProcess(argv[i]);
    }
  } else {
    reader = new ORSocketReader(readerArg.substr(0, iColon).c_str(), 
                                atoi(readerArg.substr(iColon+1).c_str()));
  }

  if (!reader->OKToRead()) {
    ORLog(kError) << "Reader couldn't read" << endl;
    return 1;
  }

  ORLog(kRoutine) << "Setting up data processing manager..." << endl;
  ORDataProcManager dataProcManager(reader);

  OR1DHistoHistogramsDecoder histDecoder;
  ORDataProcessor histProc(&histDecoder);
  histProc.SetDebugRecord();
  dataProcManager.AddProcessor(&histProc);

  ORAD3511ADCDecoder ad3511ADCDecoder;
  ORDataProcessor adc3511ADCProc(&ad3511ADCDecoder);
  adc3511ADCProc.SetDebugRecord();
  dataProcManager.AddProcessor(&adc3511ADCProc);

  ORAD413ADCDecoder ad413ADCDecoder;
  ORDataProcessor ad413ADCProc(&ad413ADCDecoder);
  ad413ADCProc.SetDebugRecord();
  dataProcManager.AddProcessor(&ad413ADCProc);

  ORAD811ADCDecoder ad811ADCDecoder;
  ORDataProcessor ad811ADCProc(&ad811ADCDecoder);
  ad811ADCProc.SetDebugRecord();
  dataProcManager.AddProcessor(&ad811ADCProc);

  ORAugerFLTEnergyDecoder augerFLTEnergyDecoder;
  ORDataProcessor augerFLTProc(&augerFLTEnergyDecoder);
  augerFLTProc.SetDebugRecord();
  dataProcManager.AddProcessor(&augerFLTProc);

  //ORCaen775tdcDecoder
  //ORCaen785adcDecoder
  //ORCaen792qdcDecoder
  //ORDGF4cEventDecoder
  //ORDGF4cMCADecoder
  //ORL2551ScalersDecoder
  //ORL4532channelTriggerDecoder
  //ORL4532mainTriggerDecoder
  //ORLC950ScopeDataDecoder
  //ORRunDecoder
  //ORShaperShaperDecoder
  //ORTDC3377tdcDecoder
  //ORTek754DScopeDataDecoder
  //ORTrigger32ClockDecoder
  //ORTrigger32LiveTimeDecoder
  //ORVTrigger32GTIDDecoder

  ORLog(kRoutine) << "Start processing..." << endl;
  dataProcManager.ProcessDataStream();
  ORLog(kRoutine) << "Finished processing..." << endl;

  delete reader;

  return 0;
}

