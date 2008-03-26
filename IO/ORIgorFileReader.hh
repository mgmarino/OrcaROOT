// ORIgorFileReader.hh

#ifndef _ORIgorFileReader_hh_
#define _ORIgorFileReader_hh_

#include <string>
#include "ORFileReader.hh"
//! Class to read Igor binary files
/*!
    This class is extremely particular, creating 
    a wrapper for Igor binary files containing DGF4c data.
    This enables DGF4c files read by Igor to be processed
    by OrcaRoot.
 */
class ORIgorFileReader : public ORFileReader 
{
  public:
    ORIgorFileReader(std::string filename = "");
    virtual ~ORIgorFileReader() {}

    //! this doesn't mean anything for IGOR 
    virtual int GetRunNumber() { return 0; } 

    //! disabling this member function. 
    virtual size_t ReadPartialLineWithCR(char* , size_t ) { return 0;} 
    virtual bool ReadRecord(UInt_t*& buffer, size_t& nLongsMax);

  protected:
    virtual std::string ReadHeader();

};

#endif
