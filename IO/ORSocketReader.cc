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
  if (fIsThreadRunning) StopThread();
  if (fCircularBuffer.buffer) delete [] fCircularBuffer.buffer;
  pthread_attr_destroy(&fThreadAttr);
  pthread_mutex_destroy(&fCircularBuffer.cbMutex);
}

void ORSocketReader::Initialize()
{
  fIsThreadRunning = false;
  pthread_mutex_init(&fCircularBuffer.cbMutex, NULL);
  fCircularBuffer.buffer = NULL;
  SetCircularBufferLength(kDefaultBufferLength);
  pthread_attr_init(&fThreadAttr);
  pthread_attr_setdetachstate(&fThreadAttr, PTHREAD_CREATE_JOINABLE);
  fThreadStatus = 0;
  fLocalBuffer.buffer = NULL;
  
}

void ORSocketReader::AddSocket(TSocket* sock)
{
  if (!fIsThreadRunning) {
    /* poor man's mutex. */
    ORMonitor::AddSocket(sock, kRead);
  }
} 

bool ORSocketReader::StartThread()
{
  if (fIsThreadRunning || GetActive() == 0) {
    return false;
  }

  TList* listOfSockets = GetListOfActives();
  TIter next(listOfSockets);
  TSocket* sock;
  while ((sock = (TSocket*)next())) {
    if (!sock->IsValid()) {
      /* One of the sockets isn't valid, don't continue. */
      delete listOfSockets;
      return false;
    }
  }
  delete listOfSockets;

  ResetCircularBuffer();
  fIsThreadRunning = true;
  fThreadStatus = 0;

  Int_t retValue = pthread_create(&fThreadId, 
    &fThreadAttr, SocketReadoutThread, this);
  if (retValue == 0) return true;

  fIsThreadRunning = false;
  return false; 
}

void ORSocketReader::StopThread()
{
  /* This function blocks until the thread stops. */
  if (!fIsThreadRunning) return;  
  fIsThreadRunning = false;
  pthread_join(fThreadId, 0);
  if (fThreadStatus < 0) {
    ORLog(kError) << "Thread exited with an error!" << std::endl;
  }
  if (fCircularBuffer.lostLongCount != 0) {
    ORLog(kWarning) << "There were lost records from the socket: " 
      << fCircularBuffer.lostLongCount << " long word" << std::endl;
  }
}

void ORSocketReader::ResetCircularBuffer()
{
  if (fCircularBuffer.buffer) delete [] fCircularBuffer.buffer;
  fCircularBuffer.buffer = new UInt_t[fBufferLength];
  fCircularBuffer.bufferLength = fBufferLength;
  fCircularBuffer.amountInBuffer = 0;
  fCircularBuffer.writeIndex = 0;
  fCircularBuffer.readIndex = 0;
  fCircularBuffer.lostLongCount = 0;
  fCircularBuffer.wrapArounds = 0;
  fLocalBuffer.bufferLength = fBufferLength >> 4;
  if (fLocalBuffer.buffer) delete [] fLocalBuffer.buffer;
  fLocalBuffer.buffer = new UInt_t[fLocalBuffer.bufferLength];
  fLocalBuffer.currentAmountOfData = 0; 
  fLocalBuffer.readIndex = 0;
}

size_t ORSocketReader::ReadFromCircularBuffer(UInt_t* buffer, size_t numLongWords)
{
  size_t firstRead = 0;
  size_t secondRead = 0;
  pthread_mutex_lock(&fCircularBuffer.cbMutex);

  if (numLongWords > fCircularBuffer.amountInBuffer) {
    /* Block until there's something to read. */
    while(1) {
      /* In this while loop, we sleep to wait for data available.  If any data are 
         available it grabs as much as it can after one sleep cycle.  If nothing
         is in the circular buffer, it waits. */
      /* unlock before we wait to not deadlock. */
      pthread_mutex_unlock(&fCircularBuffer.cbMutex);
      if (fSleepTime != 0) sleep(fSleepTime);
      /* Relock and keep it if we break out of this loop. */
      pthread_mutex_lock(&fCircularBuffer.cbMutex);
      /* There is a sufficient amount in the buffer, read it out. */
      if (numLongWords <= fCircularBuffer.amountInBuffer) break;
      /* If fThreadStatus does not equal 0, the thread is dead.  Read out
         the remainder. */
      /* If we have exceeded the number of wait cycles, just grab what we can. */
      if (fThreadStatus != 0 || fCircularBuffer.amountInBuffer != 0) {
        numLongWords = fCircularBuffer.amountInBuffer; 
        break;
      }
    }
  }

  if (numLongWords == 0) {
    pthread_mutex_unlock(&fCircularBuffer.cbMutex);
    return 0;
  }

  firstRead = (fCircularBuffer.readIndex > fCircularBuffer.writeIndex) ?
    fCircularBuffer.bufferLength - fCircularBuffer.readIndex :
    fCircularBuffer.writeIndex - fCircularBuffer.readIndex;

  if (firstRead > numLongWords) firstRead = numLongWords;
  secondRead = (firstRead < numLongWords) ? numLongWords - firstRead : 0;

  /* Now copy out. */
  memcpy(buffer, fCircularBuffer.buffer + fCircularBuffer.readIndex, firstRead*4);
  fCircularBuffer.readIndex += firstRead;
  fCircularBuffer.amountInBuffer -= firstRead;

  if (fCircularBuffer.readIndex == fCircularBuffer.bufferLength) {
    fCircularBuffer.readIndex = 0;
  }
  if (secondRead != 0) {
    memcpy(buffer+firstRead*4, 
      fCircularBuffer.buffer + fCircularBuffer.readIndex, secondRead*4);
    fCircularBuffer.readIndex += secondRead;
    fCircularBuffer.amountInBuffer -= secondRead;
  }
  if (fCircularBuffer.lostLongCount != 0) {
    /* Give a quick notification. */
    ORLog(kWarning) << "Socket has thrown away " << fCircularBuffer.lostLongCount 
      << " long words" << std::endl;
  }
  pthread_mutex_unlock(&fCircularBuffer.cbMutex);
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
  while (numLongsToRead !=0) {
    numToRead = fLocalBuffer.currentAmountOfData - fLocalBuffer.readIndex;
    if (numToRead == 0) {
      fLocalBuffer.currentAmountOfData = 
        ReadFromCircularBuffer(fLocalBuffer.buffer, fLocalBuffer.bufferLength);
      fLocalBuffer.readIndex = 0;
      /* The following indicates the circular buffer is empty and won't fill up.*/
      if (fLocalBuffer.currentAmountOfData == 0) break; 
    }
    numToRead = 
      (numLongsToRead > fLocalBuffer.currentAmountOfData - fLocalBuffer.readIndex) ? 
      fLocalBuffer.currentAmountOfData - fLocalBuffer.readIndex : numLongsToRead;
    memcpy(buffer, fLocalBuffer.buffer + fLocalBuffer.readIndex, 4*numToRead); 
    numLongsToRead -= numToRead;
    fLocalBuffer.readIndex += numToRead;
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

  Int_t numRead = 0, numLongsToRead = 0;
  const Long_t timeout = 1000;  // allows the socket to be stopped. 
  Int_t amountAbleToRead = 0;
  Int_t firstRead = 0;
  Int_t secondRead = 0;
  const size_t sizeOfScratchBuffer = 0xFFFF;
  UInt_t scratchBuffer[sizeOfScratchBuffer];
  ORSocketReader* socketReader = reinterpret_cast<ORSocketReader*>(input);

  if (socketReader == NULL) pthread_exit((void *) -1);
  while (socketReader->fIsThreadRunning && socketReader->GetActive() > 0) {
    /* In this thread, we readout the socket into the circular buffer. */
    TSocket* sock = socketReader->Select(timeout);
    if (sock == (TSocket*)-1) continue;
    if (!sock) continue; 
    numRead = sock->RecvRaw(scratchBuffer, 4, kPeek);
    if (numRead <= 0) {
      socketReader->ResetSocket(sock); 
      continue;
    }
    if (ORUtils::MustSwap()) ORUtils::Swap(scratchBuffer[0]);
    numLongsToRead = socketReader->fBasicDecoder.LengthOf(scratchBuffer);
    
    /* We're beginning to work on the circular buffer. */
    pthread_mutex_lock(&socketReader->fCircularBuffer.cbMutex);

    amountAbleToRead = socketReader->fCircularBuffer.bufferLength 
      - socketReader->fCircularBuffer.amountInBuffer; 

    /* Dealing with a record if we don't have room for it. */
    if (amountAbleToRead < numLongsToRead) {
      /* Do we wait, or throw away data? */
      /* Throw away data now. */
      socketReader->fCircularBuffer.lostLongCount += numLongsToRead;
      pthread_mutex_unlock(&socketReader->fCircularBuffer.cbMutex);
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

    if (firstRead > numLongsToRead) firstRead = numLongsToRead;

    secondRead = (firstRead < numLongsToRead) ? numLongsToRead - firstRead : 0;
    numRead = sock->RecvRaw(socketReader->fCircularBuffer.buffer 
      + socketReader->fCircularBuffer.writeIndex, firstRead*4); 

    if (numRead <= 0 || numRead != firstRead*4) {
      pthread_mutex_unlock(&socketReader->fCircularBuffer.cbMutex);
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
        socketReader->ResetSocket(sock); 
        pthread_mutex_unlock(&socketReader->fCircularBuffer.cbMutex);
        continue;
      } 
      socketReader->fCircularBuffer.writeIndex += secondRead;
      socketReader->fCircularBuffer.amountInBuffer += secondRead;
      socketReader->fCircularBuffer.wrapArounds++;
    }
    /* OK, we're done reading into the circular buffer, unlock it. */
    pthread_mutex_unlock(&socketReader->fCircularBuffer.cbMutex);
     
  }
  socketReader->fThreadStatus = 1;
  pthread_exit((void *) 0);
}

