// ORSisCviFileReader.cc

#include "ORSisCviFileReader.hh"

#include <string>
#include <cmath>
#include <sstream>
#include "ORLogger.hh"
#include "ORUtils.hh"
#include "ORBasicDataDecoder.hh"

using namespace std;

ORSisCviFileReader::ORSisCviFileReader(string filename) : ORFileReader(filename),
  fFileNumber(0), fN3302Modules(0), fRawSampleLength(0), fEnergySampleLength(0),
  fBucketPosition(0)
{
}

size_t ORSisCviFileReader::Read(char* buffer, size_t nBytesMax)
{
  read(buffer, nBytesMax);
  return gcount();
}

bool ORSisCviFileReader::ReadRecord(vector<UInt_t>& buffer)
{
  // First, read in the header if necessary. This is signaled by fStreamVersion
  // still having value kUnknownVersion
  if(fStreamVersion == ORHeaderDecoder::kUnknownVersion) {
    string header = ReadHeader();
    UInt_t nBytes = header.size() + 1;
    UInt_t headerRecordLength = ((UInt_t) ceil(double(nBytes)/4.0)) + 2;
    buffer.resize(headerRecordLength);
    buffer[0] = headerRecordLength;
    buffer[1] = nBytes;
    memcpy((&buffer[0])+2, header.c_str(), nBytes);
    /* Here we assume that the CVI files come from a little endian machine (PC). */
    fStreamVersion = ORHeaderDecoder::kOld;
    if(ORUtils::SysIsLittleEndian()) {
      fMustSwap = false;
    } else {
      fMustSwap = true;
    }
    return true;
  }

  if(fBucketPosition == fBucket.size()) {
    if(!ReadBucket()) return false;
  }

  UInt_t recordLength = 6 + (fRawSampleLength+1)/2 + fEnergySampleLength;
  if(fBucketPosition + recordLength > fBucket.size()) {
    ORLog(kWarning) << "ReadRecord(): Mismatch between fBucketPosition ("
                    << fBucketPosition << "), recordLength (" << recordLength
                    << "), and fBucket.size() (" << fBucket.size() << ")"
                    << endl;
    return false;
  }
  UInt_t trailer = kEventTrailer;
  if(fMustSwap) ORUtils::Swap(trailer);
  bool isWrapMode = false;
  if(fBucket[fBucketPosition + recordLength-1] != trailer) {
    recordLength += 2;
    isWrapMode = true;
  }
  if(fBucket[fBucketPosition + recordLength-1] != trailer) {
    ORLog(kWarning) << "ReadRecord(): event trailer not found where expected."
                    << " Scanning the bucket from position " << fBucketPosition << endl;
    while(fBucket[fBucketPosition] != trailer) {
      fBucketPosition++;
      if(fBucketPosition >= fBucket.size()) {
        ORLog(kWarning) << "ReadRecord(): event trailer not found in bucket." << endl;
        return false;
      }
      ORLog(kDebug) << "bucket[" << fBucketPosition << "] = " 
                    << fBucket[fBucketPosition] << endl;
    }
    ORLog(kWarning) << "ReadRecord(): next event trailer found at position "
                    << fBucketPosition << " (recordLength = " 
                    << recordLength << ")" << endl;
  }


  UInt_t bufferSize = recordLength + 4; // allow for the 4 ORCA header words
  buffer.resize(bufferSize);

  // Word 0 of an ORCA data record gives the length of the record
  // in 32-bit words and the data ID.  
  buffer[0] = bufferSize += 262144; 
  // Word 1 has Crate, Card, Channel, and wrap mode flag. 
  // Get the channel number from the data
  UInt_t channel = fBucket[fBucketPosition];
  if(fMustSwap) ORUtils::Swap(channel);
  channel = (channel & 0x7);
  buffer[1] = (channel << 8) | isWrapMode; 
  // Word 2 contains the number of 32-bit words used to pack the waveform
  buffer[2] = (fRawSampleLength+1)/2;
  // Word 3 contains the length of the energy filter waveform
  buffer[3] = fEnergySampleLength;
  if(fMustSwap) {
    for(size_t i=0; i<4; i++) ORUtils::Swap(buffer[i]);
  }
  // After that comes the data record from the card
  for(size_t i=0; i<recordLength; i++) {
    buffer[i+4] = fBucket[fBucketPosition + i];
  }
  fBucketPosition += recordLength;

  return OKToRead();
}

string ORSisCviFileReader::ReadHeader()
{
  ORLog(kTrace) << "Starting ReadHeader()..." << endl;
  if (!OKToRead()) return "";
  static const int kSisCviHeaderLength = 16;
  UInt_t buffer[kSisCviHeaderLength];
  size_t nBytesToRead = kSisCviHeaderLength*sizeof(UInt_t);
  size_t nBytesRead = Read((char*)buffer, nBytesToRead);
  if(nBytesRead != nBytesToRead) {
    ORLog(kWarning) << "ReadHeader(): attempt to read " << nBytesToRead
                    << " B only returned " << nBytesRead << "B " << endl;
    return "";
  }
  for (int i=0; i<kSisCviHeaderLength; i++) {
    if(fMustSwap) ORUtils::Swap(buffer[i]);
  }
  fFileNumber = buffer[0];
  UInt_t fileDataFormat = buffer[1];
  fN3302Modules = buffer[2];
  if(fN3302Modules != 1) {
    ORLog(kWarning) << "This code needs to be upgraded to read more than 1 module at a time" << endl;
  }
  UInt_t sis3820ClockMode = buffer[3] >> 16;
  UInt_t sis3302ClockMode = buffer[3] & 0xFFFF;
  UInt_t rawSampleStartIndex = buffer[4] >> 16;
  fRawSampleLength = buffer[4] & 0xFFFF;
  UInt_t decimationMode = buffer[5] >> 28;
  UInt_t firEnergyGap = (buffer[5] >> 8) & 0xFF;
  UInt_t firEnergyLength = buffer[5] & 0xFF;
  UInt_t energySampleMode = buffer[6];
  UInt_t readoutMode = buffer[7];
  UInt_t energySampleStartIndex1 = buffer[8] >> 16;
  UInt_t energySampleStartIndex2 = buffer[8] & 0xFFFF;
  UInt_t energySampleStartIndex3 = buffer[9] & 0xFFFF;
  fEnergySampleLength = buffer[9] >> 16;
  UInt_t energyGateLength = buffer[10];
  UInt_t triggerGateLength = buffer[11];
  UInt_t preTriggerDelay = buffer[12];

  ostringstream os;
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
     << "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" << endl
     << "<plist version=\"1.0\">" << endl
     << "<dict>" << endl
     << "	<key>Document Info</key>" << endl
     << "	<dict>" << endl
     << "		<key>OrcaVersion</key>" << endl
     << "		<string>SIS CVI</string>" << endl
     << "		<key>dataVersion</key>" << endl
     << "		<integer>3</integer>" << endl
     << "		<key>date</key>" << endl
     << "		<string>" << GetFileDate() << "</string>" << endl
     << "		<key>documentName</key>" << endl
     << "		<string>" << GetFilePath() << "SISCVIDataFolder</string>" << endl
     << "	</dict>" << endl
     << "	<key>Header Length</key>" << endl
     << "	<integer>@HeaderLength@</integer>" << endl
     << "	<key>ObjectInfo</key>" << endl
     << "	<dict>" << endl
     << "		<key>Crates</key>" << endl
     << "		<array>" << endl
     << "			<dict>" << endl
     << "				<key>Cards</key>" << endl
     << "				<array>" << endl
     << "					<dict>" << endl
     << "						<key>Card</key>" << endl
     << "						<integer>0</integer>" << endl
     << "						<key>Class Name</key>" << endl
     << "						<string>ORSIS3302Model</string>" << endl
     << "						<key>FileNumber</key>" << endl
     << "						<integer>" << fFileNumber << "</integer>" << endl
     << "						<key>FileDataFormat</key>" << endl
     << "						<integer>" << fileDataFormat << "</integer>" << endl
     << "						<key>NumberOf3302Modules</key>" << endl
     << "						<integer>" << fN3302Modules << "</integer>" << endl
     << "						<key>SIS3820ClockMode</key>" << endl
     << "						<integer>" << sis3820ClockMode << "</integer>" << endl
     << "						<key>SIS3302ClockMode</key>" << endl
     << "						<integer>" << sis3302ClockMode << "</integer>" << endl
     << "						<key>RawSampleStartIndex</key>" << endl
     << "						<integer>" << rawSampleStartIndex << "</integer>" << endl
     << "						<key>RawSampleLength</key>" << endl
     << "						<integer>" << fRawSampleLength << "</integer>" << endl
     << "						<key>DecimationMode</key>" << endl
     << "						<integer>" << decimationMode << "</integer>" << endl
     << "						<key>FIREnergyGap</key>" << endl
     << "						<integer>" << firEnergyGap << "</integer>" << endl
     << "						<key>FIREnergyLength</key>" << endl
     << "						<integer>" << firEnergyLength << "</integer>" << endl
     << "						<key>EnergySampleMode</key>" << endl
     << "						<integer>" << energySampleMode << "</integer>" << endl
     << "						<key>ReadoutMode</key>" << endl
     << "						<integer>" << readoutMode << "</integer>" << endl
     << "						<key>EnergySampleStartIndex1</key>" << endl
     << "						<integer>" << energySampleStartIndex1 << "</integer>" << endl
     << "						<key>EnergySampleStartIndex2</key>" << endl
     << "						<integer>" << energySampleStartIndex2 << "</integer>" << endl
     << "						<key>EnergySampleStartIndex3</key>" << endl
     << "						<integer>" << energySampleStartIndex3 << "</integer>" << endl
     << "						<key>EnergySampleLength</key>" << endl
     << "						<integer>" << fEnergySampleLength << "</integer>" << endl
     << "						<key>EnergyGateLength</key>" << endl
     << "						<integer>" << energyGateLength << "</integer>" << endl
     << "						<key>TriggerGateLength</key>" << endl
     << "						<integer>" << triggerGateLength << "</integer>" << endl
     << "						<key>PreTriggerDelay</key>" << endl
     << "						<integer>" << preTriggerDelay << "</integer>" << endl
     << "					</dict>" << endl
     << "				</array>" << endl
     << "				<key>ClassName</key>" << endl
     << "				<string>ORVme64CrateModel</string>" << endl
     << "				<key>CrateNumber</key>" << endl
     << "				<integer>0</integer>" << endl
     << "				<key>FirstSlot</key>" << endl
     << "				<integer>0</integer>" << endl
     << "			</dict>" << endl
     << "		</array>" << endl
     << "		<key>DataChain</key>" << endl
     << "		<array>" << endl
     << "			<dict>" << endl
     << "				<key>Run Control</key>" << endl
     << "				<dict>" << endl
     << "					<key>Class Name</key>" << endl
     << "					<string>ORRunModel</string>" << endl
     << "					<key>RunNumber</key>" << endl
     << "					<integer>" << fFileNumber << "</integer>" << endl
     << "					<key>quickStart</key>" << endl
     << "					<false/>" << endl
     << "					<key>refTime</key>" << endl
     << "					<real>" << GetFileRefTime() << "</real>" << endl
     << "					<key>remoteControl</key>" << endl
     << "					<false/>" << endl
     << "					<key>runType</key>" << endl
     << "					<integer>0</integer>" << endl
     << "					<key>startTime</key>" << endl
     << "					<integer>" << GetFileTime() << "</integer>" << endl
     << "				</dict>" << endl
     << "			</dict>" << endl
     << "		</array>" << endl
     << "	</dict>" << endl
     << "	<key>dataDescription</key>" << endl
     << "	<dict>" << endl
     << "		<key>ORRunModel</key>" << endl
     << "		<dict>" << endl
     << "			<key>Run</key>" << endl
     << "			<dict>" << endl
     << "				<key>dataId</key>" << endl
     << "				<integer>1310720</integer>" << endl
     << "				<key>decoder</key>" << endl
     << "				<string>ORRunDecoderForRun</string>" << endl
     << "				<key>length</key>" << endl
     << "				<integer>4</integer>" << endl
     << "				<key>variable</key>" << endl
     << "				<false/>" << endl
     << "			</dict>" << endl
     << "		</dict>" << endl
     << "		<key>ORSIS3302</key>" << endl
     << "		<dict>" << endl
     << "			<key>Energy</key>" << endl
     << "			<dict>" << endl
     << "				<key>dataId</key>" << endl
     << "				<integer>262144</integer>" << endl
     << "				<key>decoder</key>" << endl
     << "				<string>ORSIS3302DecoderForEnergy</string>" << endl
     << "				<key>length</key>" << endl
     << "				<integer>-1</integer>" << endl
     << "				<key>variable</key>" << endl
     << "				<true/>" << endl
     << "			</dict>" << endl
     << "			<key>MCA</key>" << endl
     << "			<dict>" << endl
     << "				<key>dataId</key>" << endl
     << "				<integer>524288</integer>" << endl
     << "				<key>decoder</key>" << endl
     << "				<string>ORSIS3302DecoderForMca</string>" << endl
     << "				<key>length</key>" << endl
     << "				<integer>-1</integer>" << endl
     << "				<key>variable</key>" << endl
     << "				<true/>" << endl
     << "			</dict>" << endl
     << "		</dict>" << endl
     << "	</dict>" << endl
     << "</dict>" << endl
     << "</plist>" << endl;
  if (ORLogger::GetSeverity() <= ORLogger::kDebug) {
    ORLog(kDebug) << os.str();
  }
  return os.str();
}

UInt_t ORSisCviFileReader::ReadWord(bool swapIfNecessary)
{
  UInt_t dummy;
  Read((char*) &dummy, sizeof(UInt_t));
  if(swapIfNecessary && fMustSwap) ORUtils::Swap(dummy);
  return dummy;
}

bool ORSisCviFileReader::ReadBucket()
{
  if (!OKToRead()) {
    ORLog(kWarning) << "Can't read file but EOF delimiter was not reached." << endl;
    return false;
  }
  UInt_t bucketHeader = ReadWord(true);
  if(bucketHeader == kEOF) {
    ORLog(kRoutine) << "Reached EOF." << endl;
    return false;
  }
  else if(bucketHeader != kBucketHeader) {
    ORLog(kWarning) << "Malformed bucket header 0x" << hex << bucketHeader 
                    << ", scanning for next valid header" << endl;
    size_t wordsLost = 0;
    while(bucketHeader != kBucketHeader) {
      wordsLost++;
      if (!OKToRead() || bucketHeader == kEOF) {
        ORLog(kWarning) << "Reached EOF before another valid bucket was found." 
                        << wordsLost*4 << "B lost." << endl;
        return false;
      }
      bucketHeader = ReadWord(true);
    }
    ORLog(kWarning) << "Recovered. " << wordsLost*4 << "B lost." << endl;
  }
  
  ReadWord(); // skip bucket number
  ReadWord(); // skip reserved word
  UInt_t bucketLength = ReadWord(true);
  fBucket.resize(bucketLength);
  for(size_t i=0; i<bucketLength; i++) {
    fBucket[i] = ReadWord(false);
  }
  UInt_t bucketTrailer = fBucket[bucketLength-1];
  if(fMustSwap) ORUtils::Swap(bucketTrailer);
  if(bucketTrailer != kEventTrailer) {
    ORLog(kWarning) << "Bucket ends with 0x" << hex << bucketTrailer 
                    << " instead of 0x" << hex << kEventTrailer << endl;
  }
  fBucketPosition = 0;
  return true;
}
