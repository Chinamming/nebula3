#pragma once
#ifndef POSIX_POSIXEVENT_H
#define POSIX_POSIXEVENT_H
//------------------------------------------------------------------------------
/**
    @class Posix::PosixEvent

    Posix implmentation of an event synchronization object.

    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include <semaphore.h>

//------------------------------------------------------------------------------
namespace Posix
{
class PosixEvent
{
public:
    /// constructor
    PosixEvent();
    /// destructor
    ~PosixEvent();
    /// signal the event
    void Signal();
    /// wait for the event to become signalled
    void Wait();
    /// wait for the event with timeout in millisecs
    bool WaitTimeout(int ms);
    /// check if event is signalled
    bool Peek();
private:
    sem_t semaphore;
};

//------------------------------------------------------------------------------
/**
*/
inline
PosixEvent::PosixEvent()
{
    sem_init(&(this->semaphore), 0, 0);
}

//------------------------------------------------------------------------------
/**
*/
inline
PosixEvent::~PosixEvent()
{
    sem_destroy(&(this->semaphore));
}

//------------------------------------------------------------------------------
/**
*/
inline void
PosixEvent::Signal()
{
    sem_post(&(this->semaphore));
}

//------------------------------------------------------------------------------
/**
*/
inline void
PosixEvent::Wait()
{
    sem_wait(&(this->semaphore));
}

//------------------------------------------------------------------------------
/**
    Waits for the event to become signaled with a specified timeout
    in milli seconds. If the method times out it will return false,
    if the event becomes signalled within the timeout it will return 
    true.
*/
inline bool
PosixEvent::WaitTimeout(int timeoutInMilliSec)
{
    while (timeoutInMilliSec > 0)
    {
        if (0 == sem_trywait(&(this->semaphore)))
        {
            return true;
        }
        usleep(1000);
        timeoutInMilliSec -= 1;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    This checks if the event is signalled and returnes immediately.
*/
inline bool
PosixEvent::Peek()
{
    return 0 == sem_trywait(&(this->semaphore));
}

}; // namespace Posix
//------------------------------------------------------------------------------
#endif

