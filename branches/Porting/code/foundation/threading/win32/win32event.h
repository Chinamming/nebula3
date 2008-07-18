#pragma once
#ifndef WIN32_WIN32EVENT_H
#define WIN32_WIN32EVENT_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32Event

    Win32 implmentation of an event synchronization object.

    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32Event
{
public:
    /// constructor
    Win32Event();
    /// destructor
    ~Win32Event();
    /// signal the event
    void Signal();
    /// wait for the event to become signalled
    void Wait() const;
    /// wait for the event with timeout in millisecs
    bool WaitTimeout(int ms) const;
    /// check if event is signalled
    bool Peek() const;
private:
    HANDLE event;
};

//------------------------------------------------------------------------------
/**
*/
inline
Win32Event::Win32Event()
{
    this->event = CreateEvent(NULL, FALSE, FALSE, NULL);
    n_assert(0 != this->event);
}

//------------------------------------------------------------------------------
/**
*/
inline
Win32Event::~Win32Event()
{
    n_assert(0 != this->event);
    CloseHandle(this->event);
    this->event = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32Event::Signal()
{
    n_assert(0 != this->event);
    SetEvent(this->event);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32Event::Wait() const
{
    n_assert(0 != this->event);
    WaitForSingleObject(this->event, INFINITE);
}

//------------------------------------------------------------------------------
/**
    Waits for the event to become signaled with a specified timeout
    in milli seconds. If the method times out it will return false,
    if the event becomes signalled within the timeout it will return 
    true.
*/
inline bool
Win32Event::WaitTimeout(int timeoutInMilliSec) const
{
    n_assert(0 != this->event);
    DWORD res = WaitForSingleObject(this->event, timeoutInMilliSec);
    return (WAIT_TIMEOUT == res) ? false : true;
}

//------------------------------------------------------------------------------
/**
    This checks if the event is signalled and returnes immediately.
*/
inline bool
Win32Event::Peek() const
{
    n_assert(0 != this->event);
    DWORD res = WaitForSingleObject(this->event, 0);
    return (WAIT_TIMEOUT == res) ? false : true;
}

}; // namespace Win32
//------------------------------------------------------------------------------
#endif

