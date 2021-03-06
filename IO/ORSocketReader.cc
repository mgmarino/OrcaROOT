// ORSocketReader.cc

#include "ORSocketReader.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/errno.h>
#include <unistd.h>


ORSocketReader::ORSocketReader(const char* host, int port, bool writable) 
{
  Initialize();
  fSocket = new TSocket(host, port);
  fIOwnSocket = true;
  if (writable) fSocketToWrite = fSocket;
  else fSocketToWrite = NULL;
  fSocketIsOK = true;
}

ORSocketReader::ORSocketReader(TSocket* aSocket, bool writable)
{
  Initialize();
  fSocket = aSocket;
  fIOwnSocket = false;
  if (writable) fSocketToWrite = fSocket;
  else fSocketToWrite = NULL;
  fSocketIsOK = true;
}

ORSocketReader::~ORSocketReader()
{
  StopThread();
  if (fCircularBuffer.buffer) delete [] fCircularBuffer.buffer;
  pthread_attr_destroy(&fThreadAttr);
  pthread_rwlock_destroy(&fCircularBuffer.cbMutex);
  if (fIOwnSocket) delete fSocket; 
}

void ORSocketReader::Initialize()
{
  memset(&fCircularBuffer, 0, sizeof(fCircularBuffer));
  fCircularBuffer.buffer = NULL;
  fCircularBuffer.isRunning = false;
  pthread_rwlock_init(&fCircularBuffer.cbMutex, NULL);

  SetCircularBufferLength(kDefaultBufferLength);

  pthread_attr_init(&fThreadAttr);
  pthread_attr_setdetachstate(&fThreadAttr, PTHREAD_CREATE_JOINABLE);
  memset(&fLocalBuffer, 0, sizeof(fLocalBuffer));
  fLocalBuffer.buffer = NULL;
  fSleepTime = 1;
}

bool ORSocketReader::ThreadIsStillRunning()
{
  bool threadIsStillRunning;
  pthread_rwlock_rdlock(&fCircularBuffer.cbMutex);
  threadIsStillRunning = fCircularBuffer.isRunning;
  pthread_rwlock_unlock(&fCircularBuffer.cbMutex);
  return threadIsStillRunning;
}

bool ORSocketReader::StartThread()
{
  if (ThreadIsStillRunning()) return true;
  if (!fSocketIsOK || TestCancel()) {
    return false;
  }
  if (!fSocket->IsValid()) return false;

  ResetCircularBuffer();
  fCircularBuffer.isRunning = true;

  Int_t retValue = pthread_create(&fThreadId, 
    &fThreadAttr, SocketReadoutThread, this);
  if (retValue == 0) {
    return true;
  }

  fCircularBuffer.isRunning = false;
  return false; 
}

void ORSocketReader::StopThread()
{
  /* This function blocks until the thread stops. */
  if (!ThreadIsStillRunning()) return;  
  Int_t retValue = 0;
  if ((retValue = pthread_cancel(fThreadId)) != 0) {
    /* Some pthread implementations will return an error if the thread has already quit. */
  } else {
    // block until the thread actual stops.
    pthread_join(fThreadId, 0);
  }
  fCircularBuffer.isRunning = false;
}

void ORSocketReader::ResetCircularBuffer()
{
  /* Not thread safe, be careful! */
  if (fCircularBuffer.buffer) delete [] fCircularBuffer.buffer;
  fCircularBuffer.buffer = new UInt_t[fBufferLength];
  fCircularBuffer.bufferLength = fBufferLength;
  fCircularBuffer.amountInBuffer = 0;
  fCircularBuffer.writeIndex = 0;
  fCircularBuffer.readIndex = 0;
  fCircularBuffer.lostLongCount = 0;
  fCircularBuffer.wrapArounds = 0;
  fCircularBuffer.isRunning = false;
  fLocalBuffer.bufferLength = fBufferLength >> 4;
  if (fLocalBuffer.buffer) delete [] fLocalBuffer.buffer;
  fLocalBuffer.buffer = new UInt_t[fLocalBuffer.bufferLength];
  fLocalBuffer.currentAmountOfData = 0; 
  fLocalBuffer.readIndex = 0;
}

Int_t ORSocketReader::WriteBuffer(const void* buffer, size_t nBytes)
{
  if (!fSocketToWrite) return 0;
  fSocketToWrite->Select(TSocket::kWrite); 
  Int_t numBytes;
  do { 
    numBytes = fSocketToWrite->SendRaw(buffer, nBytes);
  } while (numBytes == -4);
  return numBytes;
}

size_t ORSocketReader::ReadFromCircularBuffer(UInt_t* buffer, size_t numLongWords, size_t minimumWords)
{
  size_t firstRead = 0, secondRead = 0;
  size_t tempReadIndex = 0, tempNumWords = 0;
  pthread_rwlock_rdlock(&fCircularBuffer.cbMutex);

  if (numLongWords > fCircularBuffer.amountInBuffer) {
    /* Block until there's something to read. */
    while(1) {
      /* In this while loop, we sleep to wait for data available.  If any data are 
         available it grabs as much as it can after one sleep cycle.  If nothing
         is in the circular buffer, it waits. */
      /* unlock before we wait to not deadlock. */
      pthread_rwlock_unlock(&fCircularBuffer.cbMutex);
      if (fSleepTime != 0) sleep(fSleepTime);
      /* Relock and keep it if we break out of this loop. */
      pthread_rwlock_rdlock(&fCircularBuffer.cbMutex);
      /* There is a sufficient amount in the buffer, read it out. */
      if (numLongWords <= fCircularBuffer.amountInBuffer) break;
      /* If we have exceeded the number of wait cycles, just grab what we can. */
      if (!fCircularBuffer.isRunning || fCircularBuffer.amountInBuffer >= minimumWords) {
        numLongWords = fCircularBuffer.amountInBuffer; 
        break;
      }
      if (TestCancel()) return 0; // We've been canceled, get out
    }
  }

  if (numLongWords == 0) {
    return 0;
    pthread_rwlock_unlock(&fCircularBuffer.cbMutex);
  }
  tempNumWords = 0;
  tempReadIndex = fCircularBuffer.readIndex;
  firstRead = (fCircularBuffer.readIndex > fCircularBuffer.writeIndex) ?
    fCircularBuffer.bufferLength - fCircularBuffer.readIndex :
    fCircularBuffer.writeIndex - fCircularBuffer.readIndex;

  if (firstRead > numLongWords) firstRead = numLongWords;
  secondRead = (firstRead < numLongWords) ? numLongWords - firstRead : 0;

  /* Now copy out. */
  memcpy(buffer, fCircularBuffer.buffer + tempReadIndex, firstRead*4);
  tempReadIndex += firstRead;
  tempNumWords += firstRead;

  if (tempReadIndex == fCircularBuffer.bufferLength) {
    tempReadIndex = 0;
  }
  if (secondRead != 0) {
    memcpy(buffer + firstRead, 
      fCircularBuffer.buffer + tempReadIndex, secondRead*4);
    tempReadIndex += secondRead;
    tempNumWords += secondRead;
  }
  if (fCircularBuffer.lostLongCount != 0) {
    /* Give a quick notification. */
    ORLog(kWarning) << "Socket has thrown away " << fCircularBuffer.lostLongCount 
      << " long words" << std::endl;
  }
  pthread_rwlock_unlock(&fCircularBuffer.cbMutex);

  /* Now we write. */
  pthread_rwlock_wrlock(&fCircularBuffer.cbMutex);
  fCircularBuffer.readIndex = tempReadIndex;
  fCircularBuffer.amountInBuffer -= tempNumWords;
  fCircularBuffer.lostLongCount = 0;
  pthread_rwlock_unlock(&fCircularBuffer.cbMutex);

  return numLongWords;
}

size_t ORSocketReader::Read(char* buffer, size_t nBytes)
{
  if(nBytes==0) return nBytes;
  if(nBytes % 4 != 0) {
    ORLog(kWarning) << "Request for bytes: " << nBytes << " not a factor of 4" 
      << std::endl;
    return 0; 
  }
  
  /* We read in from a linear buffer that we fill up when it empties.  */
  /* This is to avoid obtaining mutex locks all the time. */

  size_t numLongsToRead = nBytes/4;
  size_t numToRead = 0; 
  size_t bufferPosition = 0;
  while (numLongsToRead !=0) {
    numToRead = fLocalBuffer.currentAmountOfData - fLocalBuffer.readIndex;
    if (numToRead == 0) {
      fLocalBuffer.currentAmountOfData = 
        ReadFromCircularBuffer(fLocalBuffer.buffer, fLocalBuffer.bufferLength, numLongsToRead);
      fLocalBuffer.readIndex = 0;
      /* The following indicates the circular buffer is empty and won't fill up.*/
      if (fLocalBuffer.currentAmountOfData == 0) break; 
    }
    numToRead = 
      (numLongsToRead > fLocalBuffer.currentAmountOfData - fLocalBuffer.readIndex) ? 
      fLocalBuffer.currentAmountOfData - fLocalBuffer.readIndex : numLongsToRead;
    memcpy(buffer + bufferPosition, fLocalBuffer.buffer + fLocalBuffer.readIndex, 4*numToRead); 
    numLongsToRead -= numToRead;
    fLocalBuffer.readIndex += numToRead;
    bufferPosition += 4*numToRead;
  }
  return (nBytes - 4*numLongsToRead);
}

/* These are private functions designed to be used by SocketReadoutThread and the class */
Int_t WriteToRootSocket(TSocket* sock, void* buffer, Int_t length)
{
    Int_t socketDescriptor = sock->GetDescriptor();
    Int_t numBytesWritten;
    Int_t totalBytesWritten = 0;
    char* charBuffer = (char*) buffer;
    while (length > 0) {
      // This cycles around a would block status
      fd_set fileDescSet;
      struct timeval timeout;
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      FD_ZERO(&fileDescSet);
      FD_SET(socketDescriptor, &fileDescSet);

      numBytesWritten = select(socketDescriptor+1, 0, &fileDescSet, 0, &timeout);
      if (numBytesWritten == 0) continue; // timeout
      if (numBytesWritten < 0) { 
        if (errno == EAGAIN) continue;
        numBytesWritten = 0;
        break;
      }
      /* We get here, it means data can be sent. */
      numBytesWritten = send(socketDescriptor, charBuffer, length, 0);
      if (numBytesWritten > 0) {
        length -= numBytesWritten;
        charBuffer += numBytesWritten;
        totalBytesWritten += numBytesWritten;
      } else if (numBytesWritten == 0) break;
      // Cycle around.  If there's another problem, it should be caught by select
    } 
    return totalBytesWritten;


}
Int_t ReadoutRootSocket(TSocket* sock, void* buffer, Int_t length, 
                        const ORSocketReader& check_cancel,
                        ESendRecvOptions opt = kDefault) 
{
    Int_t socketDescriptor = sock->GetDescriptor();
    Int_t numBytesRead;
    Int_t totalBytesRead = 0;
    char* charBuffer = (char*) buffer;
    while (length > 0) {
      // This cycles around a would block status
      fd_set fileDescSet;
      struct timeval timeout;
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      FD_ZERO(&fileDescSet);
      FD_SET(socketDescriptor, &fileDescSet);

      numBytesRead = select(socketDescriptor+1, &fileDescSet, 0, 0, &timeout);
      if (numBytesRead == 0) {
        if (check_cancel.TestCancel()) break;
        continue; // timeout
      }
      if (numBytesRead < 0) { 
        if (errno == EAGAIN) continue;
        numBytesRead = 0;
        break;
      }
      /* We get here, it means data is available. */
      if (opt == kPeek) {
        numBytesRead = recv(socketDescriptor, charBuffer, length, MSG_PEEK);
      } else {
        numBytesRead = recv(socketDescriptor, charBuffer, length, MSG_WAITALL);
      }
      if (numBytesRead > 0) {
        length -= numBytesRead;
        charBuffer += numBytesRead;
        totalBytesRead += numBytesRead;
      } else if (numBytesRead == 0) break;
      // Cycle around.  If there's another problem, it should be caught by select
    } 
    return totalBytesRead;

}

void* SocketReadoutThread(void* input)
{
  /* Readout Thread which sucks info out of socket as fast as it can. */
  /* Currently, it throws data away that it can't process. */
  
  bool firstWordRead = false;
  bool mustSwap = false;
  Int_t numBytesRead = 0, numLongsToRead = 0, amountAbleToRead = 0, firstRead = 0,
    secondRead = 0;
  const size_t sizeOfScratchBuffer = 0xFFFF;
  UInt_t scratchBuffer[sizeOfScratchBuffer];
  ORSocketReader* socketReader = reinterpret_cast<ORSocketReader*>(input);

  if (socketReader == NULL) pthread_exit((void *) -1);

  /* Here we set up the select function.  We do not use the ROOT version because
     it emits Events which are not pleasant to deal with in Threads. */

  TSocket* sock = socketReader->fSocket;
  //fd_set fileDescriptors; 

  while (socketReader->fSocketIsOK) {
    /* In this thread, we readout the socket into the circular buffer. */
    pthread_testcancel(); // When we are here, it is safe to cancel the thread.

    numBytesRead = ReadoutRootSocket(sock, scratchBuffer, 
                                sizeof(UInt_t), *socketReader, kPeek);
    if (numBytesRead <= 0) {
      // Something is wrong with the socket
      socketReader->fSocketIsOK = false;
      break;
    }

    // Only do the following once:
    /* Check the first word to determine if we must swap. */
    if (!firstWordRead) {
      ORHeaderDecoder headerDec;
      ORHeaderDecoder::EOrcaStreamVersion version = 
        headerDec.GetStreamVersion(scratchBuffer[0]);
      if (version == ORHeaderDecoder::kOld) {
        mustSwap = ORUtils::SysIsLittleEndian();
      }
      else if (version == ORHeaderDecoder::kNewUnswapped) {
        mustSwap = false; 
      }
      else if (version == ORHeaderDecoder::kNewSwapped) {
        mustSwap = true;
      }
      else if (version == ORHeaderDecoder::kUnknownVersion) {
        /* Get out, something is wrong. */
        socketReader->fSocketIsOK = false;
        break;
      }
      firstWordRead = true;
    }

    // Check to see the number of words to read out for this record
    if (mustSwap) ORUtils::Swap(scratchBuffer[0]);
    numLongsToRead = socketReader->fBasicDecoder.LengthOf(scratchBuffer);
    
    /* We're beginning to work on the circular buffer. */
    pthread_rwlock_rdlock(&socketReader->fCircularBuffer.cbMutex);

    amountAbleToRead = socketReader->fCircularBuffer.bufferLength 
      - socketReader->fCircularBuffer.amountInBuffer; 

    /*************************************************************/
    /* Dealing with a record if we don't have room for it. */
    /*************************************************************/
    if (amountAbleToRead < numLongsToRead) {
      /* Do we wait, or throw away data? */
      /* Throw away data now. */
      // fixME, this needs to be set in an option
      pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);
      pthread_rwlock_wrlock(&socketReader->fCircularBuffer.cbMutex);
      socketReader->fCircularBuffer.lostLongCount += numLongsToRead;
      pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);

      while (numLongsToRead > 0) {
        numBytesRead = ReadoutRootSocket(sock, scratchBuffer, 
          (((size_t)numLongsToRead > sizeOfScratchBuffer) ? 
            sizeOfScratchBuffer : numLongsToRead)*sizeof(UInt_t), *socketReader);
        if (numBytesRead <=0 || numBytesRead % sizeof(UInt_t) != 0) break;
        numLongsToRead -= numBytesRead/sizeof(UInt_t);
      }
      if (numBytesRead <= 0 || numBytesRead % sizeof(UInt_t) != 0) {
        socketReader->fSocketIsOK = false;
      }
      continue;
    }
    /*************************************************************/
    /*************************************************************/

    /*************************************************************/
    /* We have room for it, so now figure out how to read it out.*/
    /*************************************************************/

    firstRead = (socketReader->fCircularBuffer.writeIndex >= 
                 socketReader->fCircularBuffer.readIndex) ?
      socketReader->fCircularBuffer.bufferLength 
        - socketReader->fCircularBuffer.writeIndex :
      socketReader->fCircularBuffer.readIndex 
        - socketReader->fCircularBuffer.writeIndex;

    
    pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);

    /* First and second reads are set to make sure the wrapping works 
       correctly. */
    if (firstRead > numLongsToRead) firstRead = numLongsToRead;
    secondRead = (firstRead < numLongsToRead) ? numLongsToRead - firstRead : 0;

    pthread_rwlock_wrlock(&socketReader->fCircularBuffer.cbMutex);

    /* ROOT automatically calls the receive function so that we
       don't have to cycle through the calls. That is, it *will*
       get all the bytes requested or error. */
    numBytesRead = ReadoutRootSocket(sock, 
              socketReader->fCircularBuffer.buffer 
              + socketReader->fCircularBuffer.writeIndex, 
              firstRead*sizeof(UInt_t), *socketReader); 
    if (numBytesRead <= 0 || numBytesRead != (Int_t)(firstRead*sizeof(UInt_t))) {
      // Problem in the socket, or closed connection. 
      pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);
      socketReader->fSocketIsOK = false;
      break;
    } 
    socketReader->fCircularBuffer.writeIndex += numBytesRead/sizeof(UInt_t);
    socketReader->fCircularBuffer.amountInBuffer += numBytesRead/sizeof(UInt_t);

    if (socketReader->fCircularBuffer.writeIndex 
        == socketReader->fCircularBuffer.bufferLength) {
      socketReader->fCircularBuffer.writeIndex = 0;
    }

    if (secondRead != 0) {
      numBytesRead = ReadoutRootSocket(sock, 
                socketReader->fCircularBuffer.buffer 
                + socketReader->fCircularBuffer.writeIndex,
                secondRead*sizeof(UInt_t), *socketReader); 
      if (numBytesRead <= 0 || numBytesRead != (Int_t)(secondRead*sizeof(UInt_t))) {
        // Problem in the socket, or closed connection. 
        pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);
        socketReader->fSocketIsOK = false;
        break;
      } 
      socketReader->fCircularBuffer.writeIndex += numBytesRead/sizeof(UInt_t);
      socketReader->fCircularBuffer.amountInBuffer += numBytesRead/sizeof(UInt_t);
      socketReader->fCircularBuffer.wrapArounds++;
    }

    /* OK, we're done reading into the circular buffer, unlock it. */
    pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);
     
  }

  pthread_rwlock_wrlock(&socketReader->fCircularBuffer.cbMutex);
  socketReader->fCircularBuffer.isRunning = false;
  pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);

  pthread_exit((void *) 0);
}

