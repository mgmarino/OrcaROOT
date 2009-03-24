// ORVReader.hh

#ifndef _ORVWriter_hh_
#define _ORVWriter_hh_
//! Virtual Writer class defining the interface for OrcaROOT Writers.
/*!
    If a reader can also be written to (e.g. it is socket like), then the 
    reader can derive from this class and implement the function WriteBuffer
 */

class ORVWriter
{
  public:
    ORVWriter() {}
    virtual ~ORVWriter() {}

    //! Returns number of bytes written.
    virtual Int_t WriteBuffer(const void* buffer, size_t nBytes) = 0;
};

#endif
