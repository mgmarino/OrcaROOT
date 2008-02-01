// ORReadWriteLock.hh

#ifndef _ORReadWriteLock_hh
#define _ORReadWriteLock_hh

#include <pthread.h>
class ORReadWriteLock
{
  public:
    ORReadWriteLock() 
      { pthread_rwlock_init(&fRWLock, 0); }
    ~ORReadWriteLock() 
      { pthread_rwlock_destroy(&fRWLock); }

     void readLock()
       { pthread_rwlock_rdlock(&fRWLock); }

     void writeLock()
       { pthread_rwlock_wrlock(&fRWLock); }

     void unlock()
       { pthread_rwlock_unlock(&fRWLock); }

  private:
    pthread_rwlock_t fRWLock; 

};

#endif
