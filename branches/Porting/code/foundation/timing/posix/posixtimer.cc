//------------------------------------------------------------------------------
//  posixtimer.cc
//  (C) 2007 Oleg Khryptul (Haron)
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "timing/posix/posixtimer.h"

namespace Posix
{

//------------------------------------------------------------------------------
/**
*/
PosixTimer::PosixTimer() :
    running(false),
    diffTime(0),
    stopTime(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Start the timer. This will update the diffTime member to reflect
    the accumulated time when the timer was not running (basically the
    difference between this timer's time and the real system time).
*/
void
PosixTimer::Start()
{
    n_assert(!this->running);

    // query the current real time and update the diffTime member
    // to take the "lost" time into account since the timer was stopped
    clock_t curRealTime;
    curRealTime = clock();
    this->diffTime += curRealTime - this->stopTime;
    this->stopTime = 0;
    this->running = true;
}

//------------------------------------------------------------------------------
/**
    Stop the timer. This will record the current realtime, so that
    the next Start() can measure the time lost between Stop() and Start()
    which must be taken into account to keep track of the difference between
    this timer's time and realtime.
*/
void
PosixTimer::Stop()
{
    n_assert(this->running);
    this->stopTime = clock();
    this->running = false;
}

//------------------------------------------------------------------------------
/**
    Reset the timer so that will start counting at zero again.
*/
void
PosixTimer::Reset()
{
    bool wasRunning = this->running;
    if (wasRunning)
    {
        this->Stop();
    }
    this->stopTime = 0;
    this->diffTime = 0;
    if (wasRunning)
    {
        this->Start();
    }
}

//------------------------------------------------------------------------------
/**
    Returns true if the timer is currently running.
*/
bool
PosixTimer::Running() const
{
    return this->running;
}

//------------------------------------------------------------------------------
/**
    This returns the internal local time as large integer.
*/
clock_t
PosixTimer::InternalTime() const
{
    // get the current real time
    clock_t inttime;
    if (this->running)
    {
        // we are running, query current time
        inttime = clock();
    }
    else
    {
        // we are stopped, use time at last stop
        inttime = this->stopTime;
    }

    // convert to local time
    inttime -= this->diffTime;

    return inttime;
}

//------------------------------------------------------------------------------
/**
    This returns the timer's current time in seconds.
*/
Timing::Time
PosixTimer::GetTime() const
{
    Timing::Time seconds = ((Timing::Time)this->InternalTime()) / ((Timing::Time)CLOCKS_PER_SEC);
    return seconds;
}

//------------------------------------------------------------------------------
/**
    This returns the timer's current time in "ticks".
*/
uint
PosixTimer::GetTicks() const
{
    //// get the current real time
    clock_t time = this->InternalTime();

    return time / (CLOCKS_PER_SEC / 100000);
}

} // namespace Posix

