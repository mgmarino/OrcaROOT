// ORSocketReader.hh

#ifndef _ORSocketReader_hh_
#define _ORSocketReader_hh_
// This class can not have a dictionary made for it.

#include "ORVReader.hh"
#include "ORVWriter.hh"
#include "ORVSigHandler.hh"
#ifndef __CINT__
#include <pthread.h>
#else
// Dealing with CINT
typedef struct { private: char x[SIZEOF_PTHREAD_T]; } pthread_t;
typedef struct{ private: char x[SIZEOF_PTHREAD_ATTR_T]; } pthread_attr_t;
#endif

#include "TSocket.h"

//! Struct encapsulating a circular buffer
typedef struct {
   pthread_rwlock_t cbMutex;
   UInt_t*          buffer;
   size_t           bufferLength;
   size_t           amountInBuffer;
   size_t           writeIndex;
   size_t           readIndex;
   size_t           lostLongCount;
   size_t           wrapArounds;
   bool             isRunning;
} CircularBufferStruct;

//! Struct encapsulating a linear buffer
typedef struct {
    UInt_t*         buffer;
    size_t          bufferLength;
    size_t          readIndex;
    size_t          currentAmountOfData;
} LinearBufferStruct;

extern "C" void* SocketReadoutThread(void*);

//! ORSocketReader provides a class to read data from a socket.  
/*!
    It runs a thread which takes data from a socket and fills
    a circular buffer.  This circular buffer can then 
    be read out using Read().  The class will not
    block indefinitely on a call to read from the socket.
    Instead it periodically times out to check if it has
    been canceled ( from ORVSigHandler ) and exits nicely
    if so.
 */
class ORSocketReader : public ORVReader, public ORVSigHandler, public ORVWriter
{
  friend void* SocketReadoutThread(void*);  
 
  public:
    
    //! Open a socket with host:port
    /*!
       If the socket can be written back upon
       set writable to true.  This is used for example when OrcaROOT is run
       as a daemon.
     */
    ORSocketReader(const char* host, int port, bool writable = false);

    //! Open a socket using a TSocket. 
    /*!
       If the socket can be written back upon
       set writable to true.  This is used for example when OrcaROOT is run
       as a daemon.
     */
    ORSocketReader(TSocket* aSocket, bool writable = false);
    virtual ~ORSocketReader(); 

    virtual size_t Read(char* buffer, size_t nBytes);
    virtual bool OKToRead() { return (fSocket->IsValid() && fSocketIsOK); }
    virtual bool OpenDataStream() { return StartThread(); } 
    virtual void Close() { if(TestCancel() || !fSocketIsOK) StopThread(); } 
    virtual void SetCircularBufferLength(Int_t length) 
      { fBufferLength = length; }
    enum ESocketReaderConsts {kDefaultBufferLength = 0xFFFFFF};

    //! Writes onto the socket.
    /*!
        This functionality is used with regards to requests from 
        Orca which require a response from OrcaROOT.  OrcaROOT
        responds on the socket if the opened socket has
        been flagged as writable.  Returns number of bytes written.
     */
    virtual Int_t WriteBuffer(const void* buffer, size_t nBytes);

  protected:
    ORSocketReader() {} //<disallows calling this
    void Initialize();

    //! Begin Socket readout Thread.
    bool StartThread();

    //! Stop Socket readout Thread.
    void StopThread();
    void ResetCircularBuffer();

    /*! 
        This function blocks until numLongWords are available. 
        Returns the number of bytes read, 0 if there's nothing left. 
     */
    size_t ReadFromCircularBuffer(UInt_t* buffer, size_t numLongWords, size_t minWords);
    bool ThreadIsStillRunning();
    TSocket* fSocket;
    TSocket* fSocketToWrite;
    bool fIOwnSocket;
    bool fSocketIsOK;

  private:
    /* We collect the thread management and buffers here because derived 
       classes really shouldn't be modifying them. */

    pthread_t fThreadId;
    pthread_attr_t fThreadAttr;
    Int_t fBufferLength;
    CircularBufferStruct fCircularBuffer;
    LinearBufferStruct fLocalBuffer;
    UInt_t fSleepTime;

};

#endif /* _ORSocketReader_hh_ */
