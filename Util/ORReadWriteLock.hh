// ORReadWriteLock.hh

#ifndef _ORReadWriteLock_hh
#define _ORReadWriteLock_hh

#ifndef __CINT__
#include <pthread.h>
#else
typedef char pthread_rwlock_t[SIZEOF_PTHREAD_RWLOCK_T];
#endif
//! Read/Write lock wrapper class
/*!
    This class wraps pthread_rwlock_t
 */
class ORReadWriteLock
{
  public:
    ORReadWriteLock(); 

    ~ORReadWriteLock(); 

     void readLock();
     void writeLock();
     void unlock();

  private:
    ORReadWriteLock(const ORReadWriteLock&);
    ORReadWriteLock& operator=(const ORReadWriteLock&);
    pthread_rwlock_t fRWLock; //! 

};

#endif
