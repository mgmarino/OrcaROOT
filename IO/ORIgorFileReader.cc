// ORIgorFileReader.cc

#include "ORIgorFileReader.hh"

#include <string>
#include <cmath>
#include <sstream>
#include "ORLogger.hh"
#include "ORUtils.hh"
#include "ORBasicDataDecoder.hh"
using namespace std;

ORIgorFileReader::ORIgorFileReader(std::string filename) : ORFileReader(filename),
  fRunNumber(0)
{
}

size_t ORIgorFileReader::Read(char* buffer, size_t nBytesMax)
{
  read(buffer, nBytesMax);
  return gcount();
}

bool ORIgorFileReader::ReadRecord(std::vector<UInt_t>& buffer)
{
  // First, read in the header if necessary.
  if(fStreamVersion == ORHeaderDecoder::kUnknownVersion) {
    /* We increment the run number to make sure that files don.t
     * get overwritten.  This is because the file name is taken
     * from the run number in the header. */
    fRunNumber++;
    std::string header = ReadHeader();
    UInt_t nBytes = header.size() + 1;
    UInt_t recordLength = ((UInt_t) ceil(double(nBytes)/4.0)) + 2;
    if(buffer.size() < recordLength) {
      DeleteAndResizeBuffer(buffer, recordLength);
    }
    buffer[0] = recordLength;
    buffer[1] = nBytes;
    memcpy((&buffer[0])+2, header.c_str(), nBytes);
    /* Here we assume that the igor files come from a little endian (PC) 
       machine. */
    fStreamVersion = ORHeaderDecoder::kOld;
    if(ORUtils::SysIsLittleEndian()) {
      fMustSwap = false;
    } else {
      fMustSwap = true;
    }
    return true;
  }

  ORBasicDataDecoder decoder;
  UInt_t theBuffSizeLong = 0;
  if(Read(((char*) &theBuffSizeLong), 4) != 4) return false; // XIA specific, we grab the first 32-bits
  if(MustSwap()) ORUtils::Swap(theBuffSizeLong);
  UShort_t theBuffSize = (theBuffSizeLong & 0x0000FFFF);
  
  // the first 32-bits on an Orca file gives the length of the buffer dump
  // in 32-bit words.  plugging in the appropriate values.
  // also the dataID of the DGF.
  buffer[0] = theBuffSize/2 + theBuffSize % 2 + 2;  
  buffer[0] += 262144; // this is the correct dataID for the DGF  
  // the second 32-bits has Crate ID, etc.  this is unimportant
  buffer[1] = 0x00000000; // set second 32 bits to 0
  // the 3rd 32-bits takes the 1st 32 bits read out and the rest of the
  // stream is just shifted by 64 bits.
  if(MustSwap()) ORUtils::Swap(theBuffSizeLong);
  // We need to swap back since the swapping of the record is not handled
  // here
  buffer[2] = theBuffSizeLong;

  if (!OKToRead()) return false;
  size_t longRecordLength = theBuffSize/2 + 2 + theBuffSize % 2;  
  // we're adding 2 longs to the buffer
  if (longRecordLength > buffer.size()) {
    UInt_t tmp = buffer[0];
    UInt_t tmp1 = buffer[1];
    UInt_t tmp2 = buffer[2];
    DeleteAndResizeBuffer(buffer, longRecordLength);
    buffer[0] = tmp;
    buffer[1] = tmp1;
    buffer[2] = tmp2;
  }
  size_t nBytesToRead = (theBuffSize-2)*2;  // XIA is 16 bit.
  size_t nBytesRead = Read(((char*) &buffer[0])+12, nBytesToRead);
  if (nBytesRead != nBytesToRead) {
    ORLog(kWarning) << "ReadRecord(): attempt to read " << nBytesToRead
                    << " B only returned " << nBytesRead << "B "
      	      << "(id = " << decoder.DataIdOf(&buffer[0]) << ", "
      	      << "len = " << longRecordLength << ")" << std::endl;
    return false;
  }
  return OKToRead();
}

std::string ORIgorFileReader::ReadHeader()
{
  std::ostringstream os;
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
     << "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" << endl
     << "<plist version=\"1.0\">" << endl
     << "<dict>" << endl
     << "	<key>Document Info</key>" << endl
     << "	<dict>" << endl
     << "		<key>OrcaVersion</key>" << endl
     << "		<string>5.9.6</string>" << endl
     << "		<key>dataVersion</key>" << endl
     << "		<integer>2</integer>" << endl
     << "		<key>date</key>" << endl
     << "		<string>2006-07-25 14:42:23 -0700</string>" << endl
     << "		<key>documentName</key>" << endl
     << "		<string>/Users/snodaq/Desktop/REU/REU.Orca</string>" << endl
     << "	</dict>" << endl
     << "	<key>Run Control</key>" << endl
     << "	<dict>" << endl
     << "		<key>Class Name</key>" << endl
     << "		<string>ORRunModel</string>" << endl
     << "		<key>RunNumber</key>" << endl
     << "		<integer>" << fRunNumber << "</integer>" << endl
     << "		<key>quickStart</key>" << endl
     << "		<false/>" << endl
     << "		<key>refTime</key>" << endl
     << "		<real>175556544</real>" << endl
     << "		<key>remoteControl</key>" << endl
     << "		<false/>" << endl
     << "		<key>runType</key>" << endl
     << "		<integer>0</integer>" << endl
     << "		<key>startTime</key>" << endl
     << "		<integer>1153892543</integer>" << endl
     << "	</dict>" << endl
     << "	<key>crate 0</key>" << endl
     << "	<dict>" << endl
     << "		<key>Class Name</key>" << endl
     << "		<string>ORCamacCrateModel</string>" << endl
     << "		<key>count</key>" << endl
     << "		<integer>1</integer>" << endl
     << "		<key>station 15</key>" << endl
     << "		<dict>" << endl
     << "			<key>Class Name</key>" << endl
     << "			<string>ORDGF4cModel</string>" << endl
     << "			<key>binFactor</key>" << endl
     << "			<array>" << endl
     << "				<integer>442192</integer>" << endl
     << "				<integer>442192</integer>" << endl
     << "				<integer>442192</integer>" << endl
     << "				<integer>442192</integer>" << endl
     << "			</array>" << endl
     << "			<key>cutoffEmin</key>" << endl
     << "			<array>" << endl
     << "				<integer>48976</integer>" << endl
     << "				<integer>48976</integer>" << endl
     << "				<integer>48976</integer>" << endl
     << "				<integer>48976</integer>" << endl
     << "			</array>" << endl
     << "			<key>energyFlatTop</key>" << endl
     << "			<array>" << endl
     << "				<real>0.15000000596046448</real>" << endl
     << "				<real>0.15000000596046448</real>" << endl
     << "				<real>0.15000000596046448</real>" << endl
     << "				<real>0.15000000596046448</real>" << endl
     << "			</array>" << endl
     << "			<key>energyRiseTime</key>" << endl
     << "			<array>" << endl
     << "				<real>0.10000001639127731</real>" << endl
     << "				<real>0.10000001639127731</real>" << endl
     << "				<real>0.10000001639127731</real>" << endl
     << "				<real>0.10000001639127731</real>" << endl
     << "			</array>" << endl
     << "			<key>inSync</key>" << endl
     << "			<true/>" << endl
     << "			<key>psaEnd</key>" << endl
     << "			<array>" << endl
     << "				<real>100</real>" << endl
     << "				<real>100</real>" << endl
     << "				<real>100</real>" << endl
     << "				<real>100</real>" << endl
     << "			</array>" << endl
     << "			<key>psaStart</key>" << endl
     << "			<array>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "			</array>" << endl
     << "			<key>runBehavior</key>" << endl
     << "			<integer>2</integer>" << endl
     << "			<key>station</key>" << endl
     << "			<integer>15</integer>" << endl
     << "			<key>syncWait</key>" << endl
     << "			<false/>" << endl
     << "			<key>tau</key>" << endl
     << "			<array>" << endl
     << "				<real>40</real>" << endl
     << "				<real>40</real>" << endl
     << "				<real>40</real>" << endl
     << "				<real>40</real>" << endl
     << "			</array>" << endl
     << "			<key>tauSigma</key>" << endl
     << "			<array>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "			</array>" << endl
     << "			<key>traceDelay</key>" << endl
     << "			<array>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "			</array>" << endl
     << "			<key>traceLength</key>" << endl
     << "			<array>" << endl
     << "				<real>50</real>" << endl
     << "				<real>50</real>" << endl
     << "				<real>50</real>" << endl
     << "				<real>50</real>" << endl
     << "			</array>" << endl
     << "			<key>triggerFlatTop</key>" << endl
     << "			<array>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "				<real>0.0</real>" << endl
     << "			</array>" << endl
     << "			<key>triggerRiseTime</key>" << endl
     << "			<array>" << endl
     << "				<real>0.02500000037252903</real>" << endl
     << "				<real>0.02500000037252903</real>" << endl
     << "				<real>0.02500000037252903</real>" << endl
     << "				<real>0.02500000037252903</real>" << endl
     << "			</array>" << endl
     << "			<key>triggerThreshold</key>" << endl
     << "			<array>" << endl
     << "				<real>30</real>" << endl
     << "				<real>30</real>" << endl
     << "				<real>30</real>" << endl
     << "				<real>30</real>" << endl
     << "			</array>" << endl
     << "			<key>vGain</key>" << endl
     << "			<array>" << endl
     << "				<real>9</real>" << endl
     << "				<real>9</real>" << endl
     << "				<real>9</real>" << endl
     << "				<real>9</real>" << endl
     << "			</array>" << endl
     << "			<key>vOffset</key>" << endl
     << "			<array>" << endl
     << "				<real>-0.03369140625</real>" << endl
     << "				<real>-0.03369140625</real>" << endl
     << "				<real>-0.03369140625</real>" << endl
     << "				<real>-0.03369140625</real>" << endl
     << "			</array>" << endl
     << "			<key>xwait</key>" << endl
     << "			<array>" << endl
     << "				<integer>48976</integer>" << endl
     << "				<integer>48976</integer>" << endl
     << "				<integer>48976</integer>" << endl
     << "				<integer>48976</integer>" << endl
     << "			</array>" << endl
     << "		</dict>" << endl
     << "	</dict>" << endl
     << "	<key>dataDescription</key>" << endl
     << "	<dict>" << endl
     << "		<key>ORDGF4cModel</key>" << endl
     << "		<dict>" << endl
     << "			<key>Event</key>" << endl
     << "			<dict>" << endl
     << "				<key>dataId</key>" << endl
     << "				<integer>262144</integer>" << endl
     << "				<key>decoder</key>" << endl
     << "				<string>ORDGF4cDecoderForEvent</string>" << endl
     << "				<key>length</key>" << endl
     << "				<integer>-1</integer>" << endl
     << "				<key>variable</key>" << endl
     << "				<true/>" << endl
     << "			</dict>" << endl
     << "			<key>LiveTime</key>" << endl
     << "			<dict>" << endl
     << "				<key>dataId</key>" << endl
     << "				<integer>524288</integer>" << endl
     << "				<key>decoder</key>" << endl
     << "				<string>ORDGF4cDecoderForLiveTime</string>" << endl
     << "				<key>length</key>" << endl
     << "				<integer>11</integer>" << endl
     << "				<key>variable</key>" << endl
     << "				<false/>" << endl
     << "			</dict>" << endl
     << "			<key>Waveform</key>" << endl
     << "			<dict>" << endl
     << "				<key>dataId</key>" << endl
     << "				<integer>786432</integer>" << endl
     << "				<key>decoder</key>" << endl
     << "				<string>ORDGF4cDecoderForWaveform</string>" << endl
     << "				<key>length</key>" << endl
     << "				<integer>-1</integer>" << endl
     << "				<key>variable</key>" << endl
     << "				<true/>" << endl
     << "			</dict>" << endl
     << "		</dict>" << endl
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
     << "	</dict>" << endl
     << "	<key>eventDescription</key>" << endl
     << "	<dict>" << endl
     << "		<key>ORDGF4cModel</key>" << endl
     << "		<dict>" << endl
     << "			<key>dataId</key>" << endl
     << "			<integer>262144</integer>" << endl
     << "			<key>maxChannels</key>" << endl
     << "			<integer>4</integer>" << endl
     << "			<key>name</key>" << endl
     << "			<string>Waveform</string>" << endl
     << "		</dict>" << endl
     << "	</dict>" << endl
     << "	<key>slot 0</key>" << endl
     << "	<dict>" << endl
     << "		<key>Class Name</key>" << endl
     << "		<string>ORPCICamacModel</string>" << endl
     << "		<key>slot</key>" << endl
     << "		<integer>0</integer>" << endl
     << "	</dict>" << endl
     << "</dict>" << endl
     << "</plist>" << endl;
  return os.str();
}
