// orcaroot_ipe_unc.cc

#include <stdlib.h>
#include <getopt.h>
#include <string>
#include "ORBasicTreeWriter.hh"
#include "ORDataProcManager.hh"
#include "ORFileReader.hh"
#include "ORFileWriter.hh"
#include "ORHistWriter.hh"
//#include "ORHistDrawer.hh"
#include "ORLogger.hh"
//#include "ORProcessStopper.hh" // removed -tb-
//#include "ORShaperShaperTreeWriter.hh" 
//#include "ORSocketReader.hh"
#include "ORSocketReader.hh"  // new -tb-
#include "ORHandlerThread.hh" // new -tb-



/** \page orcarootipeunc OrcaRoot IPE Unc Decoder
  * This  is the OrcaRoot IPE Unc Decoder (file orcaroot_ipe_unc.cc)
  * New classes:
  * ORIpeV4FLTWaveformDecoder and ORIpeV4FLTWaveformTreeWriter
  * ORIpeV4FLTEnergyDecoder and ORIpeV4FLTEnergyTreeWriter
  * ORIpeV4FLTHitrateDecoder and ORIpeV4FLTHitrateTreeWriter
  *
  *
  * History:
  *   - 2008-02-19 If input is EXACTLY ONE file and if it is a IPE Crate file shaper tree is omitted.
  *   - 2008-02-19 The IPE Katrin Crate files omit empty trees.
  *   - 2008-02-12 Version from Michelle.
  *   - 2008-02-06 First test version (-tb-).
  */ //-tb- 2008-02-12


  //ADDITION FOR KATRIN - Start
  // this part is for the IPE katrin crate
  #include "ORIpeV4FLTEnergyDecoder.hh"
  #include "ORIpeV4FLTEnergyTreeWriter.hh"

  #include "ORIpeV4FLTEnergyHistogramDecoder.hh"
  #include "ORIpeV4FLTEnergyHistogramTreeWriter.hh"

  #include "ORIpeV4FLTWaveformDecoder.hh"
  #include "ORIpeV4FLTWaveformDumper.hh"
  #include "ORIpeV4FLTWaveformTreeWriter.hh"

  //#include "ORIpeV4FLTHitrateDecoder.hh"
  //#include "ORIpeV4FLTHitrateTreeWriter.hh"

  //this part is for the UW crate
  #include "ORTrig4ChanDecoder.hh"
  #include "ORTrig4ChanShaperCompoundProcessor.hh"
  #include "ORTrig4ChanShaperFilter.hh"

  //ADDITION FOR KATRIN - Stop

using namespace std;

static const char Usage[] =
"\n"
"\n"
"Usage: orcaroot_ipe_unc [options] [input file(s) / socket host:port]\n"
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
"  --label [label] : use [label] as prefix for root output file name.\n"
"\n"
"Example usage:\n"
"orcaroot run194ecpu\n"
"  Rootify local file run194ecpu with default verbosity, output file label, etc.\n"
"orcaroot run*\n"
"  Same as before, but run over all files beginning with \"run\".\n"
"orcaroot --verbosity debug --label mylabel run194ecpu\n"
"  The same, but with example usage of the verbosity and mylabel options.\n"
"  An output file will be created with name mylabel_run194.root, and lots\n"
"  of debugging output will appear.\n"
"orcaroot 128.95.100.213:44666\n"
"  Rootify orca stream on host 128.95.100.213, port 44666 with default verbosity,\n"
"  output file label, etc.\n"
"\n"
"\n";


int main(int argc, char** argv)
{
  //printf("DEBUG version for KATRIN ipe crate and uw crate -tb-\n");

  if(argc == 1) {
    ORLog(kError) << "You must supply some options" << endl << Usage;
    return 1;
  }


  static struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"verbosity", required_argument, 0, 'v'},
    {"label", required_argument, 0, 'l'},
  };

  string label = "OR";
  ORVReader* reader = NULL;
  //ORProcessStopper* stopper = NULL; // removed -tb-

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
      case('l'): // label
        label = optarg;
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

  ORHandlerThread* handlerThread = new ORHandlerThread(); // new -tb-
  handlerThread->StartThread();                           // new -tb-
  bool useShaperDecoder  = true; // new -tb- 2008-02-19

  string readerArg = argv[optind];
  size_t iColon = readerArg.find(":");
  if (iColon == string::npos) {
    // BEGIN check for IPE Katrin Crate file -tb- 2008-02-19
    // 
    if(argc==2){
      //debug -tb- cout << "Filename: " << argv[1] << endl;
      TFile *tf=new TFile(argv[1],"raw");
      char buff[4096];
      tf->ReadBuffer(buff,2048);
      //debug -tb- cout << "Buffer: " << buff << endl;
      string str=buff;
      size_t iKatrinKey = str.find("ORIpeV4FLTModel");
      if(iKatrinKey != string::npos){
        ORLog(kRoutine) << "Found key " << "ORIpeV4FLTModel" << " ... is a IPE crate file ..." << endl;
        useShaperDecoder = false;
      }
      //exit(999);
    }else{
      ORLog(kRoutine) << "More than 1 inputs: Using all implemented decoders   ... " << endl;
    }    
    // END check for IPE Katrin Crate file -tb- 2008-02-19
    reader = new ORFileReader;
    for (int i=optind; i<argc; i++) {
      ((ORFileReader*) reader)->AddFileToProcess(argv[i]);
    }
  } else {
    reader = new ORSocketReader(readerArg.substr(0, iColon).c_str(), 
                                atoi(readerArg.substr(iColon+1).c_str()));
    // stopper = new ORProcessStopper; //removed -tb-
  }

  if (!reader->OKToRead()) {
    ORLog(kError) << "Reader couldn't read" << endl;
    return 1;
  }

  ORLog(kRoutine) << "Setting up data processing manager..." << endl;
  ORDataProcManager dataProcManager(reader);

  ORLog(kRoutine) << "Starting output file writer..." << endl;
  ORFileWriter fileWriter(label);
  dataProcManager.AddProcessor(&fileWriter);



  //ADDITION FOR KATRIN - Start


// -tb- 2008-02-12
  // this part is for the IPE katrin crate
  #if 0
  //ORIpeV4FLTWaveformDecoder ipeV4FLTWaveformDecoder("katrinWaveformTree");
  ORIpeV4FLTWaveformTreeWriter ipeV4WFTree("ipeV4WaveformTree");
  dataProcManager.AddProcessor(&ipeV4WFTree);
  
  //ORIpeV4FLTWaveformDecoder ipeV4FLTWaveformDecoder;
  //dataProcManager.AddProcessor(&ipeV4FLTWaveformDecoder);
  #else
  
   ORIpeV4FLTEnergyDecoder ipeV4FLTEnergyDecoder;
   ORIpeV4FLTWaveformDecoder ipeV4FLTWaveformDecoder;
   ORIpeV4FLTEnergyDecoder ipeV4FLTEnergyHistogramDecoder;

   ORIpeV4FLTEnergyTreeWriter ipeV4FLTEnergyTreeWriter("energyTree");
   dataProcManager.AddProcessor(&ipeV4FLTEnergyTreeWriter);

   ORIpeV4FLTWaveformTreeWriter ipeV4FLTWaveformTreeWriter("waveformTree");
   dataProcManager.AddProcessor(&ipeV4FLTWaveformTreeWriter);

   ORIpeV4FLTEnergyHistogramTreeWriter ipeV4FLTEnergyHistogramTreeWriter("energyHistogramTree");
   dataProcManager.AddProcessor(&ipeV4FLTEnergyHistogramTreeWriter);
  #endif
// For debugging:
//   ORIpeV4FLTWaveformDumper ipeV4FLTWaveformDumper;
//   dataProcManager.AddProcessor(&ipeV4FLTWaveformDumper);
  
  
  //this part is for the UW crate
  //ORShaperShaperDecoder shaperShaperDecoder;
  //ORTrig4ChanDecoder trig4ChanDecoder; 

  //ORBasicTreeWriter shaperShaperTreeWriter(&shaperShaperDecoder, "OldshaperTree");
  //dataProcManager.AddProcessor(&shaperShaperTreeWriter);

  //ORBasicTreeWriter trig4ChanTreeWriter(&trig4ChanDecoder, "OldtriggerTree");
  //dataProcManager.AddProcessor(&trig4ChanTreeWriter);  

  ORTrig4ChanShaperCompoundProcessor triggerShaperTreeWriter;
  if(useShaperDecoder){
    dataProcManager.AddProcessor(&triggerShaperTreeWriter);
  }
  

  //ADDITION FOR KATRIN - Stop

  ORLog(kRoutine) << "Start processing..." << endl;
  //if(stopper != NULL) stopper->ExecuteStopperThread(); // removed -tb-
  dataProcManager.ProcessDataStream();
  ORLog(kRoutine) << "Finished processing..." << endl;



  delete reader;
  delete handlerThread;
  //delete stopper;

  return 0;
}

