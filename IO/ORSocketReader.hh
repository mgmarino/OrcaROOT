// ORSocketReader.hh

#ifndef _ORSocketReader_hh_
#define _ORSocketReader_hh_

#include "ORMonitor.hh"
#include "ORVReader.hh"
#include <pthread.h>

typedef struct {
    pthread_mutex_t  cbMutex;
    UInt_t*         buffer;
    size_t          bufferLength;
    size_t          amountInBuffer;
    size_t          writeIndex;
    size_t          readIndex;
    size_t          lostLongCount;
    size_t          wrapArounds;
} CircularBufferStruct;

typedef struct {
    UInt_t*         buffer;
    size_t          bufferLength;
    size_t          readIndex;
    size_t          currentAmountOfData;
} LinearBufferStruct;

extern "C" void* SocketReadoutThread(void*);

class ORSocketReader : public ORMonitor, public ORVReader
{
  friend void* SocketReadoutThread(void*);  
 
  public:
    ORSocketReader(const char* host, int port, bool writable = false);
    ORSocketReader(TSocket* aSocket, bool writable = false);
    virtual ~ORSocketReader(); 

    virtual size_t Read(char* buffer, size_t nBytes);
    virtual bool OKToRead() 
      { return (fIsThreadRunning) ? (fThreadStatus == 0) : (GetActive() > 0); }
    virtual bool OpenDataStream() { return StartThread(); } 
    virtual void Close() { StopThread(); } 
    virtual void SetCircularBufferLength(Int_t length) 
      {fBufferLength = length;}

    enum ESocketReaderConsts {kDefaultBufferLength = 0xFFFFFF};
    virtual void AddSocket(TSocket* sock);

  protected:
    ORSocketReader() {} //disallows calling this
    void Initialize();
    bool StartThread();
    void StopThread();
    void ResetCircularBuffer();
    size_t ReadFromCircularBuffer(UInt_t* buffer, size_t numLongWords);
      /* This function blocks until numLongWords are available. */
      /* Returns the number of bytes read, 0 if there's nothing left. */


  private:
    /* We collect the thread management and buffers here because derived 
       classes really shouldn't be modifying them. */
    pthread_t fThreadId;
    Int_t fThreadStatus;
    pthread_attr_t fThreadAttr;
    bool fIsThreadRunning;
    Int_t fBufferLength;
    CircularBufferStruct fCircularBuffer;
    LinearBufferStruct fLocalBuffer;
    UInt_t fSleepTime;

};

#endif
