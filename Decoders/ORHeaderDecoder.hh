// ORHeaderDecoder.hh

#ifndef _ORHeaderDecoder_hh_
#define _ORHeaderDecoder_hh_

#include "ORVDataDecoder.hh"
#include "ORLogger.hh"

class ORHeaderDecoder : public ORVDataDecoder
{
  public:
    enum EOrcaStreamVersion { kOld, kNewUnswapped, kNewSwapped, kUnknownVersion };

    // This query should be made upon reading the first word from a data
    // stream. This function will decide whether or not the stream
    // is swapped relative to the system. If the record is not a
    // header data record, you are screwed.
    virtual EOrcaStreamVersion GetStreamVersion(UInt_t firstWordOfStream);

    virtual inline UInt_t FirstWordOldVersion() { return *((UInt_t*) "<?xm"); }
    virtual inline UInt_t ExpectedDataId() { return 0x0; }

    virtual bool IsHeader(UInt_t firstWordOfRecord);
    virtual inline UInt_t NBytesOf(UInt_t* dataRecord) 
      { return sizeof(UInt_t)*LengthOf(dataRecord); }
    // Gets number of bytes (including the 8-byte header)

    virtual inline const char* HeaderStringOf(UInt_t* dataRecord) { return (const char*) (dataRecord + 2); }
    /* Be careful here, make sure to check the length of the record. */

    virtual void Swap(UInt_t* /*dataRecord*/) {}
    /* Don't require a swap for char data. */
    virtual std::string GetDataObjectPath() { 
      ORLog(kWarning) << "GetDataObjectPath() should not be called for an ORHeaderDecoder! " 
                      << "returning an empty string." << std::endl;
      return ""; 
    }
};

#endif
