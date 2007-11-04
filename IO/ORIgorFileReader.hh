// ORIgorFileReader.hh

#ifndef _ORIgorFileReader_hh_
#define _ORIgorFileReader_hh_

#include <string>
#include "ORFileReader.hh"

class ORIgorFileReader : public ORFileReader 
{
  public:
    ORIgorFileReader(std::string filename = "");
    virtual ~ORIgorFileReader() {}

    virtual int GetRunNumber() {return 0;} // this doesn't mean anything for IGOR
    virtual size_t ReadPartialLineWithCR(char* , size_t ) { return 0;} // disabling this member function.
    virtual bool ReadRecord(UInt_t*& buffer, size_t& nLongsMax);

  protected:
    virtual std::string ReadHeader();

};

#endif
