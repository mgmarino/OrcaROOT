// ORVReader.cc

#include "ORVReader.hh"

#include <cmath>
#include <iomanip>
#include <string>
#include <cstdlib>
#include "ORBasicDataDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

ORVReader::ORVReader()
{
  fStreamVersion = ORHeaderDecoder::kUnknownVersion;
  fMustSwap = false;
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

bool ORVReader::ReadRecord(std::vector<UInt_t>& buffer)
{
  bool stillOpen = true;

  stillOpen = ReadFirstWord(buffer);
  if (!stillOpen) return false;

  // Handle header packets special-like for backwards compatibility
  if (fHeaderDecoder.IsHeader(buffer[0])) {
    ORLog(kDebug) << "Reading rest of header..." << std::endl;
    stillOpen = ReadRestOfHeader(buffer);
    if (!stillOpen) return false;
  } else if (fBasicDecoder.IsLong(&buffer[0])) {
    stillOpen = ReadRestOfLongRecord(buffer);
    if (!stillOpen) return false;
  }

  if (ORLogger::GetSeverity() <= ORLogger::kDebug) { // check severity; improves speed
    char type = 'l';
    if (fHeaderDecoder.IsHeader(buffer[0])) type = 'h';
    else if (fBasicDecoder.IsShort(&buffer[0])) type = 's';
    ORLog(kDebug) << "ReadRecord(): " << type << ": id = " 
                  << std::setw(11) << fBasicDecoder.DataIdOf(&buffer[0]) 
		  << "\tlen = " << fBasicDecoder.LengthOf(&buffer[0]) << std::endl;
  }

  return true;
}

size_t ORVReader::DeleteAndResizeBuffer(std::vector<UInt_t>& buffer, size_t newNLongsMax)
{
  buffer.resize(newNLongsMax);
  return newNLongsMax;
}

void ORVReader::DetermineFileTypeAndSetupSwap(char* buffer)
{
  fStreamVersion = fHeaderDecoder.GetStreamVersion(((UInt_t*) buffer)[0]);
  if (fStreamVersion == ORHeaderDecoder::kOld) {
    fMustSwap = ORUtils::SysIsLittleEndian();
  }
  else if (fStreamVersion == ORHeaderDecoder::kNewUnswapped) {
    fMustSwap = false; 
  }
  else if (fStreamVersion == ORHeaderDecoder::kNewSwapped) {
    fMustSwap = true;
  }
  else if (fStreamVersion == ORHeaderDecoder::kUnknownVersion) {
    ORLog(kError) << "The file type could not be determined from the header!" 
                  << std::endl << "Buffer in char was " << std::endl
                  << buffer[0] << buffer[1] << buffer[2] << buffer[3] 
                  << std::endl << "Buffer as hex was 0x" 
                  << std::hex << ((UInt_t*) buffer)[0] << std::dec << std::endl;
  }
}

bool ORVReader::ReadFirstWord(std::vector<UInt_t>& buffer)
{
  if(buffer.size() < 4) {
    DeleteAndResizeBuffer(buffer, 4);
  }
  if (Read((char*) &buffer[0], 4) != 4) return false;
  if(fStreamVersion == ORHeaderDecoder::kUnknownVersion) {
    DetermineFileTypeAndSetupSwap((char*) &buffer[0]);
  }
  if (MustSwap()) {
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



bool ORVReader::ReadRestOfHeader(std::vector<UInt_t>& buffer)
{
  if(fStreamVersion == ORHeaderDecoder::kOld) {
    size_t lineBufferSize = 1024; // max Bytes for a single line
    char* lineBuffer = new char[lineBufferSize];
    memset(lineBuffer, 0, lineBufferSize);
    memcpy(lineBuffer, &buffer[0], 4);
    ReadPartialLineWithCR(lineBuffer+4, lineBufferSize);
    std::string header = lineBuffer;
    while (std::string(lineBuffer) != "</plist>\n") {
      ReadPartialLineWithCR(lineBuffer, lineBufferSize);
      if (!OKToRead()) return false;
      //ORLog(kDebug) << "ReadLine retrieved:" << lineBuffer << std::endl;
      header += lineBuffer;
    }
    delete [] lineBuffer;
    UInt_t nBytes = header.size() + 1;
    UInt_t recordLength = ((UInt_t) ceil(double(nBytes)/4.0)) + 2;
    if(buffer.size() < recordLength) {
      DeleteAndResizeBuffer(buffer, recordLength);
    }
    ORLog(kDebug) << "ReadRestOfHeader(): rec len = " << recordLength 
                  << ", nBytes = " << nBytes << std::endl;
    buffer[0] = recordLength;
    buffer[1] = nBytes;
    memcpy(&(buffer[0])+2, header.c_str(), nBytes);
  }
  else {
    if(!ReadRestOfLongRecord(buffer)) return false;
    // char strings don't end up swapped. 
    // we have to swap the second word since Header records have
    // 2 *header* words.  Most records have only one header word. 
    if (MustSwap()) ORUtils::Swap(buffer[1]);
  }
  return true;
}



bool ORVReader::ReadRestOfLongRecord(std::vector<UInt_t>& buffer)
{
  /* No swapping happens here.  This *must* be done in the decoder. 
   * ORVReader takes care of first long word. */
  size_t longRecordLength = fBasicDecoder.LengthOf(&buffer[0]);
  if (longRecordLength > buffer.size()) {
    UInt_t tmp = buffer[0];
    DeleteAndResizeBuffer(buffer, longRecordLength*2); // factor of 2 for margin 
    buffer[0] = tmp;
  }
  if (longRecordLength < 1) { 
    ORLog(kError) << "Record length is less than one!" << std::endl;
    return false; // We will have a problem here.
  }
  size_t nBytesToRead = (longRecordLength-1)*4;
  size_t nBytesRead = Read(((char*) &buffer[0])+4, nBytesToRead);
  if (nBytesRead != nBytesToRead) {
    ORLog(kWarning) << "ReadRecord(): attempt to read " << nBytesToRead
                    << " B only returned " << nBytesRead << "B "
      	      << "(id = " << fBasicDecoder.DataIdOf(&buffer[0]) << ", "
      	      << "len = " << longRecordLength << ")" << std::endl;
    return false;
  }
  return true;
}

