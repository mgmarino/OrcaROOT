// ORSisCviFileReader.hh

#ifndef _ORSisCviFileReader_hh_
#define _ORSisCviFileReader_hh_

#include <string>
#include "ORFileReader.hh"
//! Class to read SIS CVI binary files
/*!
    This class is extremely particular, creating 
    a wrapper for SIS CVI binary files containing DGF4c data.
    This enables DGF4c files read by SIS CVI to be processed
    by OrcaRoot.
 */
class ORSisCviFileReader : public ORFileReader 
{
  public:
    ORSisCviFileReader(std::string filename = "");
    virtual ~ORSisCviFileReader() {}

    virtual size_t Read(char* buffer, size_t nBytes);
    virtual size_t ReadPartialLineWithCR(char* , size_t ) { return 0;} 
    virtual bool ReadRecord(std::vector<UInt_t>& buffer);

  protected:
    enum ESISFileDelimiters { kEOF =  0xE0F0E0F, kBucketHeader = 0xABBAABBA, kEventTrailer = 0xDEADBEEF };
    virtual UInt_t ReadWord(bool swapIfNecessary = true);
    virtual bool ReadBucket();
    virtual std::string ReadHeader();
    Int_t fFileNumber;
    Int_t fN3302Modules;
    UInt_t fRawSampleLength;
    UInt_t fEnergySampleLength;
    std::vector<UInt_t> fBucket;
    size_t fBucketPosition;
};

#endif
