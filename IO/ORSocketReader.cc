// ORSocketReader.cc

#include "ORSocketReader.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
#include "TCollection.h"

using namespace std;

ORSocketReader::ORSocketReader(const char* host, int port, bool writable) :
  ORMonitor(host, port, writable)
{
  Initialize();
  /* If the socket is writable, we don't want to sleep. */
  fSleepTime = (writable) ? 0 : 1;
}

ORSocketReader::ORSocketReader(TSocket* aSocket, bool writable) :
  ORMonitor(aSocket, writable)
{
  Initialize();
  /* If the socket is writable, we don't want to sleep. */
  fSleepTime = (writable) ? 0 : 1;
}

ORSocketReader::~ORSocketReader()
{
  StopThread();
  if (fCircularBuffer.buffer) delete [] fCircularBuffer.buffer;
  pthread_attr_destroy(&fThreadAttr);
  pthread_rwlock_destroy(&fCircularBuffer.cbMutex);
}

void ORSocketReader::Initialize()
{
  pthread_rwlock_init(&fCircularBuffer.cbMutex, NULL);
  fCircularBuffer.buffer = NULL;
  SetCircularBufferLength(kDefaultBufferLength);
  pthread_attr_init(&fThreadAttr);
  pthread_attr_setdetachstate(&fThreadAttr, PTHREAD_CREATE_JOINABLE);
  fLocalBuffer.buffer = NULL;
  
}

/*
void ORSocketReader::AddSocket(TSocket* sock)
{
  if (!fIsThreadRunning) {
    *//* poor man's mutex. */
    /*ORMonitor::AddSocket(sock, kRead);
  }
} */

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
  if (GetActive() == 0) {
    return false;
  }

  TList* listOfSockets = GetListOfActives();
  TIter next(listOfSockets);
  TSocket* sock;
  while ((sock = (TSocket*)next())) {
    if (!sock->IsValid()) {
      /* One of the sockets isn't valid, don't continue. */
      delete listOfSockets;
      ORLog(kError) << "Socket invalid!" << endl;
      return false;
    }
  }
  delete listOfSockets;

  ResetCircularBuffer();
  fCircularBuffer.isRunning = true;

  Int_t retValue = pthread_create(&fThreadId, 
    &fThreadAttr, SocketReadoutThread, this);
  if (retValue == 0) return true;

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
    /*if (retValue != ESRCH) {
      ORLog(kError) << "Thread not cancelled, error: " << strerror(retValue) << std::endl;
    } else {*/
    //}
  } else {
    // block until the thread actual stops.
    pthread_join(fThreadId, 0);
  }
  if (fCircularBuffer.lostLongCount != 0) {
    ORLog(kWarning) << "There were lost records from the socket: " 
      << fCircularBuffer.lostLongCount << " long word" << std::endl;
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
  pthread_rwlock_unlock(&fCircularBuffer.cbMutex);

  return numLongWords;
}

size_t ORSocketReader::Read(char* buffer, size_t nBytes)
{
  if(!OKToRead()) return 0;  
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

void* SocketReadoutThread(void* input)
{
  /* Readout Thread which sucks info out of socket as fast as it can. */
  /* Currently, it throws data away that it can't process. */
  /* The variable fThreadStatus holds the status of the thread and is non-zero
     when the thread has exited (negative values indicate an error).  There is
     not a mutex lock on this variable because while the thread is running no 
     other part of the class is writing it, only reading it.  Race conditions 
     won't exist with this variable. */

  bool firstWordRead = false;
  bool mustSwap = false;
  Int_t numRead = 0, numLongsToRead = 0, amountAbleToRead = 0, firstRead = 0,
    secondRead = 0;
  const Long_t timeout = 1000;  // allows the socket to be stopped. 
  const size_t sizeOfScratchBuffer = 0xFFFF;
  UInt_t scratchBuffer[sizeOfScratchBuffer];
  ORSocketReader* socketReader = reinterpret_cast<ORSocketReader*>(input);

  if (socketReader == NULL) pthread_exit((void *) -1);

  while (socketReader->GetActive() > 0) {
    /* In this thread, we readout the socket into the circular buffer. */
    pthread_testcancel(); // When we are here, it is safe to cancel the thread.
    TSocket* sock = socketReader->Select(timeout);
    if (sock == (TSocket*)-1) continue;
    if (!sock) continue; 
    numRead = sock->RecvRaw(scratchBuffer, 4, kPeek);
    if (numRead <= 0) {
      socketReader->ResetSocket(sock); 
      continue;
    }
    if (!firstWordRead) {
      /* Check the first word to determine if we must swap. */
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
        socketReader->ResetSocket(sock);
        continue;
      }
      firstWordRead = true;
    }
    if (mustSwap) ORUtils::Swap(scratchBuffer[0]);
    numLongsToRead = socketReader->fBasicDecoder.LengthOf(scratchBuffer);
    
    /* We're beginning to work on the circular buffer. */
    pthread_rwlock_rdlock(&socketReader->fCircularBuffer.cbMutex);

    amountAbleToRead = socketReader->fCircularBuffer.bufferLength 
      - socketReader->fCircularBuffer.amountInBuffer; 

    /* Dealing with a record if we don't have room for it. */
    if (amountAbleToRead < numLongsToRead) {
      /* Do we wait, or throw away data? */
      /* Throw away data now. */
      socketReader->fCircularBuffer.lostLongCount += numLongsToRead;
      pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);
      while (numLongsToRead > 0) {
        numRead = sock->RecvRaw(scratchBuffer, 
          (((size_t)numLongsToRead > sizeOfScratchBuffer) ? 
            sizeOfScratchBuffer : numLongsToRead)*4);
        if (numRead <=0 || numRead % 4 != 0) break;
        numLongsToRead -= numRead/4;
      }
      if (numRead <= 0 || numRead % 4 != 0) {
        socketReader->ResetSocket(sock); 
      }
      continue;
    }

    /* We have room for it, so now figure out how to read it out.*/
    firstRead = (socketReader->fCircularBuffer.writeIndex >= 
                 socketReader->fCircularBuffer.readIndex) ?
      socketReader->fCircularBuffer.bufferLength - socketReader->fCircularBuffer.writeIndex :
      socketReader->fCircularBuffer.readIndex - socketReader->fCircularBuffer.writeIndex;

    
    pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);
    if (firstRead > numLongsToRead) firstRead = numLongsToRead;

    secondRead = (firstRead < numLongsToRead) ? numLongsToRead - firstRead : 0;
    pthread_rwlock_wrlock(&socketReader->fCircularBuffer.cbMutex);
    numRead = sock->RecvRaw(socketReader->fCircularBuffer.buffer 
      + socketReader->fCircularBuffer.writeIndex, firstRead*4); 

    if (numRead <= 0 || numRead != firstRead*4) {
      pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);
      socketReader->ResetSocket(sock); 
      continue;
    } 

    socketReader->fCircularBuffer.writeIndex += firstRead;
    socketReader->fCircularBuffer.amountInBuffer += firstRead;

    if (socketReader->fCircularBuffer.writeIndex == socketReader->fCircularBuffer.bufferLength) {
      socketReader->fCircularBuffer.writeIndex = 0;
    }

    if (secondRead != 0) {
      numRead = sock->RecvRaw(socketReader->fCircularBuffer.buffer + socketReader->fCircularBuffer.writeIndex,
        secondRead*4); 
      if (numRead <= 0 || numRead != secondRead*4) {
        pthread_rwlock_unlock(&socketReader->fCircularBuffer.cbMutex);
        socketReader->ResetSocket(sock); 
        continue;
      } 
      socketReader->fCircularBuffer.writeIndex += secondRead;
      socketReader->fCircularBuffer.amountInBuffer += secondRead;
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

