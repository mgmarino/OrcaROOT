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
   saved to disk.  This class can not exist across threads. 
 */
class ORFileReader : public std::ifstream, public ORVReader
{
  public:
    ORFileReader(std::string filename = "");
    virtual ~ORFileReader() {}

    virtual size_t Read(char* buffer, size_t nBytesMax);
    virtual bool OKToRead() { return (fFileList.size() > 0) || 
                                     (peek() && !bad() && 
                                      !eof() && good()); }

    //! Open the next file in the file list.
    virtual bool OpenDataStream();
    virtual void Close() { close(); fCurrentFileName = ""; }

    //! Add a file to the file list.
    virtual void AddFileToProcess(std::string filename)
      { fFileList.push_back(filename); }

    //! Get last-modified-date string of current file
    virtual std::string GetFileDate();
    //! Get path of current file
    virtual std::string GetFilePath();
    //! Get last-modification UTC time of current file
    virtual int GetFileTime();
    //! Get time interval in seconds since 1 January 2001, GMT of current file
    virtual int GetFileRefTime();

  protected:
    std::vector<std::string> fFileList;
    std::string fCurrentFileName;
};

#endif
