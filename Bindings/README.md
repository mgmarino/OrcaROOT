Python Bindings
===============
- M. Marino Mar 2013

Adds python bindings for OrcaROOT via PyROOT.  This currently is *experimental*
and relies on unpublished interfaces to pyROOT (i.e. interfaces not available
in `$(ROOTSYS)/include`).  _However, newer versions of ROOT should export these
interfaces._ 

The goal of the bindings is to allow orcaroot to be run from within python and
to be able to build processors within python that use the OrcaROOT framework.
This has the advantage of yielding fast proto-typing and also not requiring
compilation of external binaries to process an ORCA file.  It also allows
gluing together external libraries, e.g. libraries that define ROOT-objects
used for persisting objects/data to disk.  The goal is to simplify the process
of using external libraries. 

Current Limitations:
- Almost all OrcaROOT objects are available in ROOT or pyROOT.  A few related
  to threading and signal handling could not have their dictionaries generated. 
- A goal of the bindings is to allow generation of objects that can be called
  within the OrcaROOT C++ processing framework.  At the moment, this is limited
  to a couple of (base) processors.  Future work may introduce the ability to
  write a decoder in python. 

Example:
Processors that may be overloaded within python are:
  - `ORPyTreeWriter` -- Deriving from `ORVTreeWriter`
  - `ORPyDataProcessor` -- Deriving from `ORDataProcessor`

```python
"""
The following is an example of how one might overload ORPyTreeWriter to
generate a tree with waveform data.
"""
import ROOT
import sys
import array
import os

# Load the python dictionaries (and all others) in.
ROOT.gSystem.Load(os.path.expandvars("$ORDIR/Bindings/libPyOrcaROOT"))

# Deriving from ORPyTreeWriter allows us to write a tree in OrcaROOT 
class WFConvert(ROOT.ORPyTreeWriter):
    def __init__(self, dec, treeName):
        # Call of the base class is important
        ROOT.ORPyTreeWriter.__init__(self, dec, treeName)

        # array provides a mechanism to store basic variables in ROOT TTrees.
        # It is also possible (easy) to have objects in a branch of a TTree.
        
        self.fEventTime = array.array('d', [0.]) 
        self.fCrate = array.array('h', [0])
        self.fCard = array.array('h', [0]) 
        self.fChannel = array.array('h', [0])
        self.fEnergy = array.array('L', [0])
        self.fFlags = array.array('L', [0])
        self.Clear()
        
        # DoNotAutoFillTree calls through to SetDoNotAutoFillTree
        self.DoNotAutoFillTree()

    def Clear(self):
        self.fEventTime[0] = 0
        self.fCrate[0] = 0
        self.fCard[0] = 0
        self.fChannel[0] = 0
        self.fEnergy[0] = 0 
        self.fFlags[0] = 0 
   
    def InitializeBranches(self):
        # GetTree() is provided in ORPyTreeWriter because fTree is protected
        t = self.GetTree()
        t.Branch("eventTime", self.fEventTime, "eventTime/D")
        t.Branch("crate", self.fCrate, "crate/s")
        t.Branch("card", self.fCard, "card/s")
        t.Branch("channel", self.fChannel, "channel/s")
        t.Branch("energy", self.fEnergy, "energy/i")
        t.Branch("flags", self.fFlags, "flags/i")
        return self.kSuccess

    def ProcessMyDataRecord(self, record):
        ed = self.GetDecoder()
        if ed is None:
            print "No decoder defined!"
            return self.kFailure

        if not ed.SetDataRecord(record): return self.kFailure

        self.fCrate[0] = ed.CrateOf()
        self.fCard[0] = ed.CardOf()
        for i in range(ed.GetNumberOfEvents()):
            self.fEventTime[0] = ed.GetEventTime(i)
            self.fChannel[0] = ed.GetEventChannel(i)
            self.fEnergy[0] = ed.GetEventEnergy(i)
            self.fFlags[0] = ed.GetEventFlags(i)
            self.GetTree().Fill()
        return self.kSuccess

def main(inputs):
    reader = ROOT.ORFileReader()
    for afile in inputs: 
      reader.AddFileToProcess(afile)

    mgr = ROOT.ORDataProcManager(reader)
    fw = ROOT.ORFileWriter("adcTest")
    run_notes = ROOT.ORRunNotesProcessor()
    sisGen = ROOT.ORSIS3302GenericDecoder()
    wf = WFConvert(sisGen, "sisDec") 

    mgr.AddProcessor(fw)
    mgr.AddProcessor(run_notes)
    mgr.AddProcessor(wf)

    mgr.ProcessDataStream()

if __name__ == "__main__":
    main(sys.argv[1:])
```
