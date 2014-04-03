#include "ORReadWriteLock.hh"

ORReadWriteLock::ORReadWriteLock() 
{ 
  pthread_rwlock_init(&fRWLock, 0); 
}

ORReadWriteLock::~ORReadWriteLock() 
{ 
  pthread_rwlock_destroy(&fRWLock); 
}

void ORReadWriteLock::readLock()
{ 
  pthread_rwlock_rdlock(&fRWLock); 
}

void ORReadWriteLock::writeLock()
{ 
  pthread_rwlock_wrlock(&fRWLock); 
}

void ORReadWriteLock::unlock()
{ 
  pthread_rwlock_unlock(&fRWLock); 
}


