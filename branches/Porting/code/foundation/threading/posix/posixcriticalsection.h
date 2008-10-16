#pragma once
#ifndef THREADING_POSIXCRITICALSECTION_H
#define THREADING_POSIXCRITICALSECTION_H
//------------------------------------------------------------------------------
/**
    @class Posix::PosixCriticalSection
  
    Posix-implementation of critical section. Critical section
    objects are used to protect a portion of code from parallel
    execution. Define a static critical section object and
    use its Enter() and Leave() methods to protect critical sections
    of your code.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include <pthread.h>

//------------------------------------------------------------------------------
namespace Posix
{
class PosixCriticalSection
{
public:
    /// constructor
    PosixCriticalSection();
    /// destructor
    ~PosixCriticalSection();
    /// enter the critical section
    void Enter();
    /// leave the critical section
    void Leave();
private:
    pthread_mutex_t mutex;
};

//------------------------------------------------------------------------------
/**
*/
inline
PosixCriticalSection::PosixCriticalSection()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(this->mutex), &attr);
    pthread_mutexattr_destroy(&attr);
}

//------------------------------------------------------------------------------
/**
*/
inline
PosixCriticalSection::~PosixCriticalSection()
{
    pthread_mutex_destroy(&(this->mutex));
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PosixCriticalSection::Enter()
{
    pthread_mutex_lock(&(this->mutex));
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PosixCriticalSection::Leave()
{
    pthread_mutex_unlock(&(this->mutex));
}

};
//------------------------------------------------------------------------------
#endif
