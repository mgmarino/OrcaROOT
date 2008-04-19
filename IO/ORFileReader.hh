// ORFileReader.hh

#ifndef _ORFileReader_hh_
#define _ORFileReader_hh_

#include <fstream>
#include <string>
#include <vector>
#include "ORVReader.hh"

//! Class to read in files
/*!
   This class reads in Orca files that have been
   saved to disk. 
 */
class ORFileReader : public std::ifstream, public ORVReader
{
  public:
    ORFileReader(std::string filename = "");
    virtual ~ORFileReader() {}

    virtual size_t Read(char* buffer, size_t nBytesMax);
    virtual bool OKToRead() { return !bad() && !eof() && good(); }

    //! Open the next file in the file list.
    virtual bool OpenDataStream();
    virtual void Close() { close(); }

    //! Add a file to the file list.
    virtual void AddFileToProcess(std::string filename)
      { fFileList.push_back(filename); }

  protected:
    std::vector<std::string> fFileList;
};

#endif
