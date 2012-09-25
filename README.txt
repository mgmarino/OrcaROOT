OrcaRoot

Created by Jason Detwiler, 2005
Updated by Michael Marino, 2007
For questions/information, contact jasondet@gmail.com, or mmarino@gmail.com.

   Pre-Installation

   Ensure the ROOT is installed correctly, with xml and threading enabled.  
   Thread should be enabled by default and xml is automatically enabled
   if ROOT's configure can find libxml2.  Also, optionally enable fftw3 
   support.

I. Installation

  1) Run ./configure [--with-rootsys=/path/to/root] 
     This should make sure your system can compile and install Orcaroot.
     A failure here should generate a message that can aid in rectifying
     the problem.

  2) make [-j#] 
     Replace # with the number of processors or cores.  This will speed
     up the compilation process.
    
  3) set env vars:
     On Mac OS X:
     setenv ORDIR [path to OrcaRoot directory, e.g. /home/OrcaRoot]
     setenv PATH $ORDIR/Applications:$PATH
     setenv DYLD_LIBRARY_PATH $ORDIR/lib:$DYLD_LIBRARY_PATH

     On Linux:
     setenv ORDIR [path to OrcaRoot directory, e.g. /home/OrcaRoot]
     setenv PATH $ORDIR/Applications:$PATH
     setenv LD_LIBRARY_PATH $ORDIR/lib:$LD_LIBRARY_PATH

  4) Set up a directory for your own work. Use the files
  $ORDIR/UserMakefileExample (rename it "Makefile) and
  $ORDIR/Applications/orcaroot.cc (rename it something like
  orcaroot_[project].cc) as examples to help get you started in
  making your own standalone executable. If you write any generally
  useful Decoders or Processors, contact me (jasondet@gmail.com)
  about adding them to the CVS repository. See Section II: General
  Description below for a description on how these executables generally
  work.

  5) Typical usage to run an orcaroot executable:
  orcaroot Run[#]
           ^^^^^^ raw data file output by ORCA 
  ^^^^^^^^ executable; may be named something else (like
  orcaroot_[project]). Should be in the $ORDIR/Applications directory.
  There are additionally several options to the provided examples
  executables. Entering just the executable (with no arguments or data
  files) will typically list the usage information and available options.
  
  6) Run OrcaROOT as daemon (optional)
  You can run OrcaROOT as daemon to do fits directly from Orca. To faciliate
  the configuration, a suitable init script for Debian GNU/Linux (also works
  on Ubuntu and probably any other LSB compliant distribution) is included.
    
    cp Applications/etc/default/orcaroot /etc/default/orcaroot
  
  Open /etc/default/orcaroot with your favourite text editor and set all
  settings according to the comments.

    cp Applications/etc/init.d/orcaroot /etc/init.d/orcaroot
    chmod a+x /etc/init.d/orcaroot

  Configure your System to run /etc/init.d/orcaroot on boot. For Debian:

    update-rc.d orcaroot defaults
  
  OrcaROOT will now be started whenever you reboot your system. To start it
  right now without rebooting, use

    /etc/init.d/orcaroot start


II. General Description

OrcaRoot is a ROOT-based toolkit of C++ class libraries that interface
with Orca data streams. Typically, OrcaRoot is used to write the
data stream directly into ROOT TTrees, histograms, and other ROOT
objects, and store them in a ROOT TFile for quick and immediate
processing. However, OrcaRoot is general enough to serve as the
Orca data stream interface for more complex event building processes,
whether or not such processes use ROOT objects or files.  OrcaRoot
can also act as a server daemon, handling real-time requests from 
ORCA to perform such tasks as curve fitting and Fourier analysis.

The reading of the Orca data stream is handled by the IO virtual
class ORVReader, which has two concrete implementations: ORFileReader
for reading from Orca files on disk, and ORSocketReader for reading
data broadcast over a network socket. At the beginning of a run,
the ORVReaders read in the Orca file header into an ORHeader object
which is stored for the rest of the run. Then the ORVReaders read
data records one by one into buffers of binary data that may then
be passed to other objects for processing.

The format of the binary data is likely different for each DAQ
component in the data stream. Therefore dedicated objects must be
created to process data from each component. In general several
different tasks might be desired for each DAQ component. For example,
one might wish to write data from a particular component to a TTree
and save it to disk AND simultaneously pass the same data to a
histogram to be displayed online. To provide a clean separation
between different tasks and to minimize replicated code, the
processing is divided among two sets of classes: decoders and
processors.  Decoders handle the conversion of the binary data into
more user-friendly data types, typically UInt_t's (see footnote 1),
although the user is free to extract the binary data into any type
of object desired.  Processors are the objects that use the decoders
to extract the data and perform a particular task. It is intended
that a separate processor be made to perform each desired task.
This improves readability, encapsulation, and configurability of
the code.

Implementing OrcaRoot for a particular DAQ setup will primarily
involve the simple task of writing a decoder for each data producing
component, followed by the more intensive task of creating a system
of processors to perform the desired tasks, and finally creating
an executable in which the processors are deployed. This last step
can usually be performed by simply copying and modifying the orcaroot
executable source code. To ease the burden on a new user, the toolkit
contains a variety of example decoders, processors, and applications.
Moreover, for simple data components such as an ADC whose contents
can be interpreted as a set of UInt_t's to be written to a TTree
or histogramed, basic processors have been written to automate the
tree writing and histogram filling tasks.  See some of the example
ADC decoders listed below that use this basic tree / histogram
writing interface. For more complicated processing, a base class
ORDataProcessor provides the following set of virtual functions to
define the processing interface:

- StartProcessing() - called before any data is read
- StartRun() - called at the start of each run
- ProcessMyDataRecord(long* record) - called whenever a data record
  associated with the processor is received; this is where typical
  processors perform the bulk of their work
- EndRun() - called at the end of each run
- EndProcessing() - called at the end of processing

By implementing the above functions appropriately, and by combining
interrelated processors into ORCompoundProcessors, most processing
tasks should be achievable.

The ORDataProcManager class performs the central management of the
data record reading loop and the issuing of the records to the
various processors active in a run. Users wishing to become familiar
with the structure of the code and the flow of processing should
attempt to read and understand the functions
ORDataProcManager::ProcessDataStream() and it's subfunction,
ORDataProcManager::ProcessRun(). It will be necessary to understand
the the EReturnCode conventions used in the ORDataProcessor interface
and their special meaning to an ORCompoundProcessor, of which
ORDataProcManager is a derived class; see ORDataProcessor.hh and
ORCompoundProcessor.hh for details.


III. Toolkit Contents

Below is a list of all of the OrcaRoot directories and a description
of their contents. The directories are listed in dependency-order,
and their contents are listed roughly in order of importance. See
the source code for details on the particular classes.

Disclaimer: I apologize in advance that not all of the source code
is properly documented.  I only hope that the code itself is clear
enough that the user can get a general idea of how it works without
the help of extensive comments. Please contact me with any questions:
jasondet@gmail.com

The following provides an outline of each major base class.  In all
cases, header files provide more extensive 

Util
- ORLogger: utility for centralized info/error logging
- ORVSigHandler: virtual base class for objects that need to perform
  special clean-up procedures on SIGINT (ctrl-c).
- ORUtils: byte-swapping utilities for cross-platform endian issues

Decoders 
- ORVDataDecoder: virtual base classes for decoders. Derived classes
  must define a function that returns a string containing the path to
  its associated data record's description in the header.  
  - Swap(): This function swaps the data when necessary (i.e. when 
    the endianness of the DAQ computer differs from that of the
    OrcaROOT computer.
  - GetDataObjectPath(): This function returns the path in the xml header
    for a particular dataId.  For example, if the dataId is located under
    <key>dataDescription</key>
    <dict>
    ...
        <key>AnObject</key>
        <dict>
        ...
            <key>DataFromObject</key>
            <dict>
                <key>dataId</key>
                <integer>9909</integer>

    ...
    then GetDataObjectPath() would return "AnObject:DataFromObject".
    It automatically searches in the dataDescription dictionary and 
    automatically adds the dataId key.
  - GetDictionaryObjectPath(): Some records include a hardware dictionary
    residing in the xml header
    that is static information associated with the hardware such as
    parameters, timing, etc.  If this function returns a non-zero sized
    string, then OrcaROOT will search for all the cards that fit this parameter.
  - For more information please see the header file.
- ORVDigitizerDecoder: This virtual class provides an interface to which all
  digitizer type record decoders should adhere.
                    
- ORBasicDataDecoder: wrapped version of ORVDataDecoder for use primarily by
  ORVReader; not associated with a particular data-producing DAQ component¬
- ORVBasicTreeDecoder: virtual base class defining interface for decoders
  that can be made to write their data to a simple TTree, where the
  branches are all UInt_t's (see ORBasicTreeWriter).  Relieves the
  user of the need to write an entire processor for this simple task.
- ORVHistDecoder: like ORVBasicTreeDecoder, but for TH1's (see
  ORHistWriter)¬
- A few data-component-specific data decoders 
  (naming convention: OR + Orca header identifier + Decoder)
  - ORRunDecoder: decodes the run data record; the name of this decoder
    does not follow the naming convention.
  - ORAD413ADCDecoder: example of ORVBasicTreeDecoders and 
    ORVHistDecoders.
  - ORAD811ADCDecoder: another example of ORVBasicTreeDecoders and
    ORVHistDecoders.
  - ORL2551ScalersDecoder: example of reading an array of data out 
    of a data record.
  - ORShaperShaperDecoder: another example of ORVBasicTreeDecoders
    and ORVHistDecoders.
  - ORTek754DScopeDataDecoder: another example of reading an array
    of data out of a data record.

IO
- ORVReader: virtual base class for readers.
- ORFileReader: reads data from an Orca file.
- ORSocketReader: reads data from a network socket.
- ORHeader: encapsulates Orca's xml-header.
- ORDictionary: represents the header's xml-tree structure.

Processors
- ORRunContext: stores global information common to all processors, for
  example the current run number, whether the run has started, etc.
- ORDataProcessor: base class for data processors. Each data processor
  holds a pointer to an ORVDataDecoder which associates the processor with
  a single data record type (except for ORUtilityProcessor, whose
  ORVDataDecoder pointer is NULL, see below). 
- ORVTreeWriter: virtual base class for processors that write data
  to TTrees -- automates the TTree building, filling (if desired), and
  writing.  In addition, it automatically adds 'default' branches, including
  run number, sub-run number and run-state description.
- ORBasicTreeWriter: processor that uses an ORVBasicTreeDecoder to write
  simple data (a list of UInt_t's) to a TTree.
- ORHistWriter: processor that uses an ORVHistDecoder to fill a histogram.
- ORUtilityProcessor: base class for processors that don't process data and
  therefore do not need access to a decoder.
- ORFileWriter: ORUtilityProcessor that handles the opening and
  closing of a ROOT TFile for each run.
- ORCompoundDataProcessor: ORUtilityProcessor that holds a list of
  ORDataProcessors, executing them in-order for each of the processing
  interface functions. ORCompoundDataProcessors can hold other
  ORCompoundDataProcessors in their list.
- A few data-component-specific processors
  (naming convention: associated decoder class name - Decoder +
  direct base class suffix, i.e. Processor or TreeWriter)
  - ORRunDataProcessor: this processor has the special task of managing the
    state of fgRunContext.
  - ORShaperShaperTreeWriter: simple tree writing example;
    ORShaperShaperDecoder is already an ORVBasicTreeDecoder so this class
    really isn't necessary, but is instructive.
  - ORL2551ScalersTreeWriter: tree writing example in which the
    tree is filled manually. This processor is obsolete as of January
    2006; one obtains the same tree by give an ORBasicTreeWriter an
    ORL2551ScalersDecoder*. Kept for backwards compatibility (in
    particular, for orcaroot_minesh).
  - ORTek754DScopeDataTreeWriter: example of writing non-simple data to a
    tree (in this case, an array of integers representing a scope trace).

Management
- ORDataProcManager: central class that manages processing.
- ORProcessStopper: manages a parallel thread process that runs a
  UI from which orcaroot can be killed nicely after processing the
  current data record or halted nicely after processing for the
  current run completes.

Applications
- orcaroot: the main (example) application. Users can either edit this
  executable to use their own processors, or use this as a starting point
  to create a custom application.
- orcaroot_minesh: application used by Minesh Bacrania, a user at UW.
- getHeaderInRootFile: shows how to extract the xml-header, which
  ORFileWriter stores in the output file, and re-load it into an ORHeader.
- writeShaperTree: example of a very simple application to write ADC values
  to a TTree.
- testStopper: tests/debugs the stopper thread.
- testUtil: hello world using ORLogger.


IV. ToDo:
- check throughput/benchmark -- is it getting all the data? Any
  bottlenecks?
- graphical display capability while running / GUI
- complex grouping schemes
- orcaroot-quit function: takes PID (or operates on all PIDs of processes
  named "orcaroot"), finds CWD of process, and puts quit file in that
  directory (to cleanly kill orcaroot remotely)
- socket read/readline should time-out if desired
- factory for analyzers; can read in from file/header entry?
- configure script
- multiple-record-type processor, OR
- compound processor that can manage, e.g., loading of info into event from
  many different data records
- ORUtils.hh contents into a namespace.
- Re-scope ORProcessStopper into a UI class (one of whose commands is to
  stop processing)
- ORSocketReader reads into a buffer, but this isn't terrible efficient for 
  a non-multi-core machine.  Fix? 


Footnotes

1. The ROOT-defined data types, especially UShort_t, UInt_t, and ULong64_t,
are preferred over c++ data types such as short, int, and long long
because the ROOT versions (supposedly) have the same size in bytes on any
platform. This is especially important when reading the ORCA data buffers,
which are packed into 32-bit words.
