// ORVReader.cc

#include "ORVReader.hh"

#include <cmath>
#include <iomanip>
#include <string>
#include "ORBasicDataDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

using namespace std;

ORVReader::ORVReader()
{
  fStreamVersion = ORHeaderDecoder::kUnknownVersion;
}

size_t ORVReader::ReadPartialLineWithCR(char* buffer, size_t nBytesMax)
{
  size_t pos = 0;
  while (pos < nBytesMax-1) {
    Read(buffer+pos, 1);
    if (buffer[pos] == '\n' || buffer[pos] == '\r') {
      pos++;
      break;
    }
    pos++;
  }
  if (buffer[pos-1] == '\r') buffer[pos-1] = '\n';
  buffer[pos] = '\0';
  return strlen(buffer);
}

bool ORVReader::ReadRecord(UInt_t*& buffer, size_t& nLongsMax)
{
  bool stillOpen = true;

  stillOpen = ReadFirstWord(buffer, nLongsMax);
  if (!stillOpen) return false;

  // Handle header packets special-like for backwards compatibility
  if (fHeaderDecoder.IsHeader(buffer[0])) {
    ORLog(kDebug) << "Reading rest of header..." << endl;
    stillOpen = ReadRestOfHeader(buffer, nLongsMax);
    if (!stillOpen) return false;
  } else if (fBasicDecoder.IsLong(buffer)) {
    stillOpen = ReadRestOfLongRecord(buffer, nLongsMax);
    if (!stillOpen) return false;
  }

  if (ORLogger::GetSeverity() <= ORLogger::kDebug) { // check severity; improves speed
    char type = 'l';
    if (fHeaderDecoder.IsHeader(buffer[0])) type = 'h';
    else if (fBasicDecoder.IsShort(buffer)) type = 's';
    ORLog(kDebug) << "ReadRecord(): " << type << ": id = " 
                  << setw(11) << fBasicDecoder.DataIdOf(buffer) 
		  << "\tlen = " << fBasicDecoder.LengthOf(buffer) << endl;
  }

  return OKToRead();
}

size_t ORVReader::DeleteAndResizeBuffer(UInt_t*& buffer, size_t newNLongsMax)
{
  delete [] buffer;
  buffer = new (nothrow) UInt_t[newNLongsMax]; // reallocation; deletion of buffer must be handled by calling procedure
  if(buffer == NULL) {
    ORLog(kFatal) << "Ran out of memory attempting to allocate buffer of length " 
                  << newNLongsMax << ". " << endl;
    exit(0);
  }
  return newNLongsMax;
}

void ORVReader::DetermineFileTypeAndSetupSwap(char* buffer)
{
  fStreamVersion = fHeaderDecoder.GetStreamVersion(((UInt_t*) buffer)[0]);
  if (fStreamVersion == ORHeaderDecoder::kOld) {
    ORUtils::SetFileIsLittleEndian(false);
  }
  else if (fStreamVersion == ORHeaderDecoder::kNewUnswapped) {
    ORUtils::SetFileIsLittleEndian(ORUtils::SysIsLittleEndian());
  }
  else if (fStreamVersion == ORHeaderDecoder::kNewSwapped) {
    ORUtils::SetFileIsLittleEndian(!ORUtils::SysIsLittleEndian());
  }
  else if (fStreamVersion == ORHeaderDecoder::kUnknownVersion) {
    ORLog(kError) << "The file type could not be determined from the header!" 
                  << endl << "Buffer in char was " << endl
                  << buffer[0] << buffer[1] << buffer[2] << buffer[3] 
                  << endl << "Buffer as hex was 0x" 
                  << hex << ((UInt_t*) buffer)[0] << dec << endl;
  }
}

bool ORVReader::ReadFirstWord(UInt_t*& buffer, size_t& nLongsMax)
{
  if(nLongsMax < 4) {
    nLongsMax = DeleteAndResizeBuffer(buffer, 4);
  }
  Read((char*) buffer, 4);
  if (!OKToRead()) return false;
  if(fStreamVersion == ORHeaderDecoder::kUnknownVersion) {
    DetermineFileTypeAndSetupSwap((char*) buffer);
  }
  if (ORUtils::MustSwap()) {
    /* We always swap the first word for records except those of old headers,
     * which are no longer being created by Orca anyways.  All other swapping
     * is the responsibility of the decoder.  See ORVDataDecoder::Swap()*/
    if(!(fStreamVersion == ORHeaderDecoder::kOld && 
         buffer[0] == fHeaderDecoder.FirstWordOldVersion())) {
      ORUtils::Swap(buffer[0]); 
    }
  }
  return true;
}



bool ORVReader::ReadRestOfHeader(UInt_t*& buffer, size_t& nLongsMax)
{
  if(fStreamVersion == ORHeaderDecoder::kOld) {
    size_t lineBufferSize = 1024; // max Bytes for a single line
    char* lineBuffer = new char[lineBufferSize];
    memset(lineBuffer, 0, lineBufferSize);
    memcpy(lineBuffer, buffer, 4);
    ReadPartialLineWithCR(lineBuffer+4, lineBufferSize);
    string header = lineBuffer;
    while (string(lineBuffer) != "</plist>\n") {
      ReadPartialLineWithCR(lineBuffer, lineBufferSize);
      if (!OKToRead()) return false;
      //ORLog(kDebug) << "ReadLine retrieved:" << lineBuffer << endl;
      header += lineBuffer;
    }
    delete [] lineBuffer;
    UInt_t nBytes = header.size() + 1;
    UInt_t recordLength = ((UInt_t) ceil(double(nBytes)/4.0)) + 2;
    if(nLongsMax < recordLength) {
      nLongsMax = DeleteAndResizeBuffer(buffer, recordLength);
    }
    ORLog(kDebug) << "ReadRestOfHeader(): rec len = " << recordLength 
                  << ", nBytes = " << nBytes << endl;
    buffer[0] = recordLength;
    buffer[1] = nBytes;
    memcpy(buffer+2, header.c_str(), nBytes);
  }
  else {
    if(!ReadRestOfLongRecord(buffer, nLongsMax)) return false;
    // char strings don't end up swapped. 
    // we have to swap the second word since Header records have
    // 2 *header* words.  Most records have only one header word. 
    if (ORUtils::MustSwap()) ORUtils::Swap(buffer[1]);
  }
  return true;
}



bool ORVReader::ReadRestOfLongRecord(UInt_t*& buffer, size_t& nLongsMax)
{
  /* No swapping happens here.  This *must* be done in the decoder. 
   * ORVReader takes care of first long word. */
  size_t longRecordLength = fBasicDecoder.LengthOf(buffer);
  if (longRecordLength > nLongsMax) {
    UInt_t tmp = buffer[0];
    nLongsMax = DeleteAndResizeBuffer(buffer, longRecordLength*2); // factor of 2 for margin 
    buffer[0] = tmp;
  }
  size_t nBytesToRead = (longRecordLength-1)*4;
  size_t nBytesRead = Read(((char*) buffer)+4, nBytesToRead);
  if (nBytesRead != nBytesToRead) {
    ORLog(kWarning) << "ReadRecord(): attempt to read " << nBytesToRead
                    << " B only returned " << nBytesRead << "B "
      	      << "(id = " << fBasicDecoder.DataIdOf(buffer) << ", "
      	      << "len = " << longRecordLength << ")" << endl;
    return false;
  }
  return true;
}

