#pragma once
#ifndef WIN32_WIN32TIMER_H
#define WIN32_WIN32TIMER_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32Timer
    
    Win32 implementation of the Time::Timer class. Under Win32, time
    measurement uses the QueryPerformanceCounter() methods.

    @todo solve multiprocessor issues of QueryPerformanceCounter()
    (different processors may return different PerformanceFrequency
    values, thus, threads should be prevented from switching between
    processors with thread affinities).
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32Timer
{
public:
    /// constructor
    Win32Timer();
    /// start/continue the timer
    void Start();
    /// stop the timer
    void Stop();
    /// reset the timer
    void Reset();
    /// return true if currently running
    bool Running() const;
    /// get current time in seconds
    Timing::Time GetTime() const;
    /// get current time in ticks
    uint GetTicks() const;

private:
    /// return internal time as 64 bit integer
    __int64 InternalTime() const;

    bool running;
    __int64 diffTime;  // accumulated time when the timer was not running
    __int64 stopTime;  // when was the timer last stopped?
};

} // namespace Win32
//------------------------------------------------------------------------------
#endif
   