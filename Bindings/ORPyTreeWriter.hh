// ORPyTreeWriter.hh

#ifndef _ORPyTreeWriter_hh_
#define _ORPyTreeWriter_hh_

#include "ORPyBinder.hh"
#include "ORVTreeWriter.hh"

class ORPyTreeWriter : public ORVTreeWriter, public ORPyBinder
{
  public:
    ORPyTreeWriter(ORVDataDecoder* dec = 0, const std::string& treeName = "") : 
      ORVTreeWriter(dec, treeName) {}
   
    // these are the functions that are typically overloaded
    EReturnCode StartProcessing();
    EReturnCode ProcessMyDataRecord(UInt_t* record);
    EReturnCode EndProcessing();

    using ORVTreeWriter::Clear;
    using TObject::Clear;
    void Clear(); 

    void DoNotAutoFillTree() { SetDoNotAutoFillTree(); } 
    
    TTree* GetTree() { return fTree; }

  protected:
    EReturnCode InitializeBranches();

    ClassDef(ORPyTreeWriter, 0)

};

#endif
