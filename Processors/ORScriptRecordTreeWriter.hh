// ORScriptRecordTreeWriter.hh

#ifndef _ORScriptRecordTreeWriter_hh_
#define _ORScriptRecordTreeWriter_hh_

#include "ORVTreeWriter.hh"
#include "TString.h"
/*! Base class handling a script record.  Derived classes should
    overload ProcessMyScriptRecord and GetScriptID (if they want 
    to limit  which scripts to process) 

    This class provides some basic functionality, essentially 
    storing the record in a string in the tree.  Derived classes
    can either extend or override this functionality.
 */

class ORScriptDecoderForRecord;
class ORScriptRecordTreeWriter : public ORVTreeWriter
{
  public:
    ORScriptRecordTreeWriter(std::string treeName = "");
    virtual ~ORScriptRecordTreeWriter();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual inline void Clear() 
      {  fXMLCharString->Resize(0); }

    //! Inheriting classes should overload this if they 
    //! wish to limit their processing by script ID. 
    virtual UShort_t GetScriptID() { return 0; } 
  protected:
    virtual EReturnCode InitializeBranches();

    //! All processing of the script record should take place here.  Inheriting 
    //! classes should overload this.
    virtual EReturnCode ProcessMyScriptRecord(UInt_t* record);

  protected:
    ORScriptDecoderForRecord* fRecordDecoder;
    TString* fXMLCharString;
};

#endif
