// ORVReader.hh

#ifndef _ORVReader_hh_
#define _ORVReader_hh_

#include "ORHeaderDecoder.hh"
#include "ORBasicDataDecoder.hh"
//! Virtual Reader class defining the interface for OrcaROOT Readers.
/*!

 */
class ORVReader 
{
  public:
    ORVReader();
    virtual ~ORVReader() {}

    /*! 
       Read attempts to place up to nBytesMax of bytes into buffer.
       This function must be implemented by derived classes. 
       Returns the number of bytes actually read 
     */
    virtual size_t Read(char* buffer, size_t nBytesMax) = 0; 

    /*! 
       Function to read-in a line of text with a carriage return.
       This is used only with respect to old Orca headers. 
     */
    virtual size_t ReadPartialLineWithCR(char* buffer, size_t nBytesMax);

    /*! 
       ReadRecord attempts to read the record pointed at by buffer.  
       The function allows buffer to be reallocated hence requires "*&".
       Returns true if successful.
     */
    virtual bool ReadRecord(UInt_t*& buffer, size_t& nLongsMax); 

    virtual bool Open() { fStreamVersion = ORHeaderDecoder::kUnknownVersion; 
                          return OpenDataStream(); }
    virtual bool OKToRead() = 0;
    virtual bool OpenDataStream() = 0;
    virtual void Close() = 0;
 
    /*!
       ORVReader determines whether or not the file entering in must be swapped.
       It swaps the first long word in a buffer since this word contains 
       information on the length of the record but it doesn't swap the remainder
       of the record.  This swapping if handled by ORVDataDecoder and ORDataProcessor.
     */
    inline bool MustSwap() { return fMustSwap; }

  protected:
    virtual size_t DeleteAndResizeBuffer(UInt_t*& buffer, size_t newNLongsMax); 
    virtual void DetermineFileTypeAndSetupSwap(char* buffer);
    virtual bool ReadFirstWord(UInt_t*& buffer, size_t& nLongsMax);
    virtual bool ReadRestOfHeader(UInt_t*& buffer, size_t& nLongsMax);
    virtual bool ReadRestOfLongRecord(UInt_t*& buffer, size_t& nLongsMax);

  protected:
    ORHeaderDecoder::EOrcaStreamVersion fStreamVersion;
    ORBasicDataDecoder fBasicDecoder;
    ORHeaderDecoder fHeaderDecoder;
    bool fMustSwap;
};

#endif
