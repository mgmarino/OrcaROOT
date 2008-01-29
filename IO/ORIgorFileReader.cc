// ORIgorFileReader.cc

#include "ORIgorFileReader.hh"

#include <string>
#include <cmath>
#include "ORLogger.hh"
#include "ORUtils.hh"
#include "ORBasicDataDecoder.hh"

using namespace std;

ORIgorFileReader::ORIgorFileReader(string filename) : ORFileReader(filename)
{
}

bool ORIgorFileReader::ReadRecord(UInt_t*& buffer, size_t& nLongsMax)
{
  if(fStreamVersion == ORHeaderDecoder::kUnknownVersion) {
    string header = ReadHeader();
    UInt_t nBytes = header.size() + 1;
    UInt_t recordLength = ((UInt_t) ceil(double(nBytes)/4.0)) + 2;
    if(nLongsMax < recordLength) {
      nLongsMax = DeleteAndResizeBuffer(buffer, recordLength);
    }
    buffer[0] = recordLength;
    buffer[1] = nBytes;
    memcpy(buffer+2, header.c_str(), nBytes);
    ORUtils::SetFileIsLittleEndian(true);
    if(ORUtils::SysIsLittleEndian()) fStreamVersion = ORHeaderDecoder::kNewUnswapped;
    else fStreamVersion = ORHeaderDecoder::kNewSwapped;
    return true;
  }

  ORBasicDataDecoder decoder;
  UInt_t theBuffSizeLong = 0;
  Read(((char*) &theBuffSizeLong), 4); // XIA specific, we grab the first 32-bits
  if(ORUtils::MustSwap()) ORUtils::Swap(theBuffSizeLong);
  UShort_t theBuffSize = (UShort_t) theBuffSizeLong;
  
  // the first 32-bits on an Orca file gives the length of the buffer dump
  // in 32-bit words.  plugging in the appropriate values.
  // also the dataID of the DGF.
  buffer[0] = theBuffSize/2 + theBuffSize % 2 + 2;  
  buffer[0] += 262144; // this is the correct dataID for the DGF  
  // the second 32-bits has Crate ID, etc.  this is unimportant
  buffer[1] = 0x00000000; // set second 32 bits to 0
  // the 3rd 32-bits takes the 1st 32 bits read out and the rest of the
  // stream is just shifted by 64 bits.
  if(ORUtils::MustSwap()) ORUtils::Swap(theBuffSizeLong);
  // We need to swap back since the swapping of the record is not handled
  // here
  buffer[2] = theBuffSizeLong;

  if (!OKToRead()) return false;
  size_t longRecordLength = theBuffSize/2 + 2 + theBuffSize % 2;  
  // we're adding 2 longs to the buffer
  if (longRecordLength > nLongsMax) {
    UInt_t tmp = buffer[0];
    UInt_t tmp1 = buffer[1];
    UInt_t tmp2 = buffer[2];
    nLongsMax = DeleteAndResizeBuffer(buffer, longRecordLength);
    buffer[0] = tmp;
    buffer[1] = tmp1;
    buffer[2] = tmp2;
  }
  size_t nBytesToRead = (theBuffSize-2)*2;  // XIA is 16 bit.
  size_t nBytesRead = Read(((char*) buffer)+12, nBytesToRead);
  if (nBytesRead != nBytesToRead) {
    ORLog(kWarning) << "ReadRecord(): attempt to read " << nBytesToRead
                    << " B only returned " << nBytesRead << "B "
      	      << "(id = " << decoder.DataIdOf(buffer) << ", "
      	      << "len = " << longRecordLength << ")" << endl;
    return false;
  }
  return OKToRead();
}

string ORIgorFileReader::ReadHeader()
{
  return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n<plist version=\"1.0\">\n<dict>\n\t<key>Document Info</key>\n\t<dict>\n\t\t<key>OrcaVersion</key>\n\t\t<string>5.9.6</string>\n\t\t<key>dataVersion</key>\n\t\t<integer>2</integer>\n\t\t<key>date</key>\n\t\t<string>2006-07-25 14:42:23 -0700</string>\n\t\t<key>documentName</key>\n\t\t<string>/Users/snodaq/Desktop/REU/REU.Orca</string>\n\t</dict>\n\t<key>Run Control</key>\n\t<dict>\n\t\t<key>Class Name</key>\n\t\t<string>ORRunModel</string>\n\t\t<key>RunNumber</key>\n\t\t<integer>1</integer>\n\t\t<key>quickStart</key>\n\t\t<false/>\n\t\t<key>refTime</key>\n\t\t<real>175556544</real>\n\t\t<key>remoteControl</key>\n\t\t<false/>\n\t\t<key>runType</key>\n\t\t<integer>0</integer>\n\t\t<key>startTime</key>\n\t\t<integer>1153892543</integer>\n\t</dict>\n\t<key>crate 0</key>\n\t<dict>\n\t\t<key>Class Name</key>\n\t\t<string>ORCamacCrateModel</string>\n\t\t<key>count</key>\n\t\t<integer>5</integer>\n\t\t<key>station 11</key>\n\t\t<dict>\n\t\t\t<key>Class Name</key>\n\t\t\t<string>ORADC2249Model</string>\n\t\t\t<key>onlineMask</key>\n\t\t\t<integer>1</integer>\n\t\t\t<key>station</key>\n\t\t\t<integer>11</integer>\n\t\t\t<key>suppressZeros</key>\n\t\t\t<false/>\n\t\t</dict>\n\t\t<key>station 12</key>\n\t\t<dict>\n\t\t\t<key>Class Name</key>\n\t\t\t<string>ORADC2249Model</string>\n\t\t\t<key>onlineMask</key>\n\t\t\t<integer>1</integer>\n\t\t\t<key>station</key>\n\t\t\t<integer>12</integer>\n\t\t\t<key>suppressZeros</key>\n\t\t\t<false/>\n\t\t</dict>\n\t\t<key>station 15</key>\n\t\t<dict>\n\t\t\t<key>Class Name</key>\n\t\t\t<string>ORDGF4cModel</string>\n\t\t\t<key>binFactor</key>\n\t\t\t<array>\n\t\t\t\t<integer>442192</integer>\n\t\t\t\t<integer>442192</integer>\n\t\t\t\t<integer>442192</integer>\n\t\t\t\t<integer>442192</integer>\n\t\t\t</array>\n\t\t\t<key>cutoffEmin</key>\n\t\t\t<array>\n\t\t\t\t<integer>48976</integer>\n\t\t\t\t<integer>48976</integer>\n\t\t\t\t<integer>48976</integer>\n\t\t\t\t<integer>48976</integer>\n\t\t\t</array>\n\t\t\t<key>energyFlatTop</key>\n\t\t\t<array>\n\t\t\t\t<real>0.15000000596046448</real>\n\t\t\t\t<real>0.15000000596046448</real>\n\t\t\t\t<real>0.15000000596046448</real>\n\t\t\t\t<real>0.15000000596046448</real>\n\t\t\t</array>\n\t\t\t<key>energyRiseTime</key>\n\t\t\t<array>\n\t\t\t\t<real>0.10000001639127731</real>\n\t\t\t\t<real>0.10000001639127731</real>\n\t\t\t\t<real>0.10000001639127731</real>\n\t\t\t\t<real>0.10000001639127731</real>\n\t\t\t</array>\n\t\t\t<key>inSync</key>\n\t\t\t<true/>\n\t\t\t<key>psaEnd</key>\n\t\t\t<array>\n\t\t\t\t<real>100</real>\n\t\t\t\t<real>100</real>\n\t\t\t\t<real>100</real>\n\t\t\t\t<real>100</real>\n\t\t\t</array>\n\t\t\t<key>psaStart</key>\n\t\t\t<array>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t</array>\n\t\t\t<key>runBehavior</key>\n\t\t\t<integer>2</integer>\n\t\t\t<key>station</key>\n\t\t\t<integer>15</integer>\n\t\t\t<key>syncWait</key>\n\t\t\t<false/>\n\t\t\t<key>tau</key>\n\t\t\t<array>\n\t\t\t\t<real>40</real>\n\t\t\t\t<real>40</real>\n\t\t\t\t<real>40</real>\n\t\t\t\t<real>40</real>\n\t\t\t</array>\n\t\t\t<key>tauSigma</key>\n\t\t\t<array>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t</array>\n\t\t\t<key>traceDelay</key>\n\t\t\t<array>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t</array>\n\t\t\t<key>traceLength</key>\n\t\t\t<array>\n\t\t\t\t<real>50</real>\n\t\t\t\t<real>50</real>\n\t\t\t\t<real>50</real>\n\t\t\t\t<real>50</real>\n\t\t\t</array>\n\t\t\t<key>triggerFlatTop</key>\n\t\t\t<array>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t\t<real>0.0</real>\n\t\t\t</array>\n\t\t\t<key>triggerRiseTime</key>\n\t\t\t<array>\n\t\t\t\t<real>0.02500000037252903</real>\n\t\t\t\t<real>0.02500000037252903</real>\n\t\t\t\t<real>0.02500000037252903</real>\n\t\t\t\t<real>0.02500000037252903</real>\n\t\t\t</array>\n\t\t\t<key>triggerThreshold</key>\n\t\t\t<array>\n\t\t\t\t<real>30</real>\n\t\t\t\t<real>30</real>\n\t\t\t\t<real>30</real>\n\t\t\t\t<real>30</real>\n\t\t\t</array>\n\t\t\t<key>vGain</key>\n\t\t\t<array>\n\t\t\t\t<real>9</real>\n\t\t\t\t<real>9</real>\n\t\t\t\t<real>9</real>\n\t\t\t\t<real>9</real>\n\t\t\t</array>\n\t\t\t<key>vOffset</key>\n\t\t\t<array>\n\t\t\t\t<real>-0.03369140625</real>\n\t\t\t\t<real>-0.03369140625</real>\n\t\t\t\t<real>-0.03369140625</real>\n\t\t\t\t<real>-0.03369140625</real>\n\t\t\t</array>\n\t\t\t<key>xwait</key>\n\t\t\t<array>\n\t\t\t\t<integer>48976</integer>\n\t\t\t\t<integer>48976</integer>\n\t\t\t\t<integer>48976</integer>\n\t\t\t\t<integer>48976</integer>\n\t\t\t</array>\n\t\t</dict>\n\t\t<key>station 18</key>\n\t\t<dict>\n\t\t\t<key>Class Name</key>\n\t\t\t<string>ORL2551Model</string>\n\t\t\t<key>onlineMask</key>\n\t\t\t<integer>7</integer>\n\t\t\t<key>station</key>\n\t\t\t<integer>18</integer>\n\t\t</dict>\n\t\t<key>station 24</key>\n\t\t<dict>\n\t\t\t<key>Class Name</key>\n\t\t\t<string>ORCC32Model</string>\n\t\t\t<key>station</key>\n\t\t\t<integer>24</integer>\n\t\t</dict>\n\t</dict>\n\t<key>dataDescription</key>\n\t<dict>\n\t\t<key>1DHisto</key>\n\t\t<dict>\n\t\t\t<key>Histograms</key>\n\t\t\t<dict>\n\t\t\t\t<key>dataId</key>\n\t\t\t\t<integer>1048576</integer>\n\t\t\t\t<key>decoder</key>\n\t\t\t\t<string>OR1DHistoDecoder</string>\n\t\t\t\t<key>length</key>\n\t\t\t\t<integer>-1</integer>\n\t\t\t\t<key>variable</key>\n\t\t\t\t<true/>\n\t\t\t</dict>\n\t\t</dict>\n\t\t<key>L2551</key>\n\t\t<dict>\n\t\t\t<key>Scalers</key>\n\t\t\t<dict>\n\t\t\t\t<key>dataId</key>\n\t\t\t\t<integer>0</integer>\n\t\t\t\t<key>decoder</key>\n\t\t\t\t<string>ORL2551DecoderForScalers</string>\n\t\t\t\t<key>length</key>\n\t\t\t\t<integer>2</integer>\n\t\t\t\t<key>variable</key>\n\t\t\t\t<false/>\n\t\t\t</dict>\n\t\t</dict>\n\t\t<key>ORDGF4cModel</key>\n\t\t<dict>\n\t\t\t<key>Event</key>\n\t\t\t<dict>\n\t\t\t\t<key>dataId</key>\n\t\t\t\t<integer>262144</integer>\n\t\t\t\t<key>decoder</key>\n\t\t\t\t<string>ORDGF4cDecoderForEvent</string>\n\t\t\t\t<key>length</key>\n\t\t\t\t<integer>-1</integer>\n\t\t\t\t<key>variable</key>\n\t\t\t\t<true/>\n\t\t\t</dict>\n\t\t\t<key>LiveTime</key>\n\t\t\t<dict>\n\t\t\t\t<key>dataId</key>\n\t\t\t\t<integer>524288</integer>\n\t\t\t\t<key>decoder</key>\n\t\t\t\t<string>ORDGF4cDecoderForLiveTime</string>\n\t\t\t\t<key>length</key>\n\t\t\t\t<integer>11</integer>\n\t\t\t\t<key>variable</key>\n\t\t\t\t<false/>\n\t\t\t</dict>\n\t\t\t<key>Waveform</key>\n\t\t\t<dict>\n\t\t\t\t<key>dataId</key>\n\t\t\t\t<integer>786432</integer>\n\t\t\t\t<key>decoder</key>\n\t\t\t\t<string>ORDGF4cDecoderForWaveform</string>\n\t\t\t\t<key>length</key>\n\t\t\t\t<integer>-1</integer>\n\t\t\t\t<key>variable</key>\n\t\t\t\t<true/>\n\t\t\t</dict>\n\t\t</dict>\n\t\t<key>ORRunModel</key>\n\t\t<dict>\n\t\t\t<key>Run</key>\n\t\t\t<dict>\n\t\t\t\t<key>dataId</key>\n\t\t\t\t<integer>1310720</integer>\n\t\t\t\t<key>decoder</key>\n\t\t\t\t<string>ORRunDecoderForRun</string>\n\t\t\t\t<key>length</key>\n\t\t\t\t<integer>4</integer>\n\t\t\t\t<key>variable</key>\n\t\t\t\t<false/>\n\t\t\t</dict>\n\t\t</dict>\n\t</dict>\n\t<key>eventDescription</key>\n\t<dict>\n\t\t<key>ORDGF4cModel</key>\n\t\t<dict>\n\t\t\t<key>dataId</key>\n\t\t\t<integer>262144</integer>\n\t\t\t<key>maxChannels</key>\n\t\t\t<integer>4</integer>\n\t\t\t<key>name</key>\n\t\t\t<string>Waveform</string>\n\t\t</dict>\n\t</dict>\n\t<key>slot 0</key>\n\t<dict>\n\t\t<key>Class Name</key>\n\t\t<string>ORPCICamacModel</string>\n\t\t<key>slot</key>\n\t\t<integer>0</integer>\n\t</dict>\n</dict>\n</plist>\n";
}
