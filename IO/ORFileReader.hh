// ORFileReader.hh

#ifndef _ORFileReader_hh_
#define _ORFileReader_hh_

#include <fstream>
#include <string>
#include "ORVReader.hh"

class ORFileReader : public std::ifstream, public ORVReader
{
  public:
    ORFileReader(std::string filename = "");
    virtual ~ORFileReader() {}

    virtual size_t Read(char* buffer, size_t nBytesMax);
    virtual bool OKToRead() { return !bad() && !eof() && good(); }
    virtual bool OpenDataStream();
    virtual void Close() { close(); }

    virtual void AddFileToProcess(std::string filename)
      { fFileList.push_back(filename); }

  protected:
    std::vector<std::string> fFileList;
};

#endif
