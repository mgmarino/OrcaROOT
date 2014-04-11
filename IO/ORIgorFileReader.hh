// ORIgorFileReader.hh

#ifndef _ORIgorFileReader_hh_
#define _ORIgorFileReader_hh_

#include <string>
#ifndef _ORFileReader_hh_
#include "ORFileReader.hh"
#endif
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

    virtual size_t Read(char* buffer, size_t nBytes);
    virtual size_t ReadPartialLineWithCR(char* , size_t ) { return 0;} 
    virtual bool ReadRecord(std::vector<UInt_t>& buffer);

  protected:
    virtual std::string ReadHeader();
    Int_t fRunNumber;

};

#endif
