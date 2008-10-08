#pragma once
#ifndef DEBUG_DEBUGTIMER_H
#define DEBUG_DEBUGTIMER_H
//------------------------------------------------------------------------------
/**
    @class Debug::DebugTimer
    
    A debug timer for measuring time spent in code blocks.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "timing/timer.h"
#include "util/stringatom.h"
#include "util/ringbuffer.h"
#include "threading/criticalsection.h"

//------------------------------------------------------------------------------
#if NEBULA3_ENABLE_PROFILING
#define _declare_timer(timer) Ptr<Debug::DebugTimer> timer;
#define _setup_timer(timer) timer = Debug::DebugTimer::Create(); timer->Setup(Util::StringAtom(#timer));
#define _discard_timer(timer) timer->Discard(); timer = 0;
#define _start_timer(timer) timer->Start();
#define _pause_timer(timer) timer->Pause();
#define _stop_timer(timer) timer->Stop();
#else
#define _declare_timer(timer)
#define _setup_timer(timer)
#define _discard_timer(timer)
#define _start_timer(timer)
#define _pause_timer(timer)
#define _stop_timer(timer)
#endif

//------------------------------------------------------------------------------
namespace Debug
{
class DebugTimer : public Core::RefCounted
{
    __DeclareClass(DebugTimer);
public:
    /// constructor
    DebugTimer();
    /// destructor
    virtual ~DebugTimer();

    /// setup the timer
    void Setup(const Util::StringAtom& timerName);
    /// discard the timer
    void Discard();
    /// return true if this timer has been setup
    bool IsValid() const;
    
    /// start or continue the timer
    void Start();
    /// pause the timer
    void Pause();
    /// stop the timer, writes sample to history
    void Stop();
    
    /// get the timer name
    const Util::StringAtom& GetName() const;
    /// get the most current sample
    Timing::Time GetSample() const;
    /// get the timer's history
    Util::Array<Timing::Time> GetHistory() const;

private:
    Threading::CriticalSection critSect;
    Util::StringAtom name;
    Timing::Timer timer;
    Util::RingBuffer<Timing::Time> history;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
DebugTimer::IsValid() const
{
    return this->name.IsValid();
}

//------------------------------------------------------------------------------
/**
*/
inline void
DebugTimer::Start()
{
    this->timer.Start();
}

//------------------------------------------------------------------------------
/**
*/
inline void
DebugTimer::Pause()
{
    this->timer.Stop();
}

//------------------------------------------------------------------------------
/**
*/
inline void
DebugTimer::Stop()
{
    this->timer.Stop();
    this->critSect.Enter();
    this->history.Add(this->timer.GetTime() * 1000.0f);
    this->critSect.Leave();
    this->timer.Reset();
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
DebugTimer::GetName() const
{
    return this->name;
}

} // namespace Debug
//------------------------------------------------------------------------------
#endif
