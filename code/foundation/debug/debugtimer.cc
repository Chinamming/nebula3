//------------------------------------------------------------------------------
//  debugtimer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debug/debugtimer.h"
#include "debug/debugserver.h"

namespace Debug
{
__ImplementClass(Debug::DebugTimer, 'DBGT', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
DebugTimer::DebugTimer() :
    history(1024)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DebugTimer::~DebugTimer()
{
    // we may need de-construct if the critical section is still taken
    this->critSect.Enter();
    n_assert(!this->IsValid());
    this->critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
void
DebugTimer::Setup(const StringAtom& timerName)
{
    n_assert(timerName.IsValid());
    n_assert(!this->IsValid());

    this->critSect.Enter();
    this->name = timerName;
    this->history.Reset();
    this->timer.Reset();
    this->critSect.Leave();

    DebugServer::Instance()->RegisterDebugTimer(this);
}

//------------------------------------------------------------------------------
/**
*/
void
DebugTimer::Discard()
{
    n_assert(this->IsValid());
    DebugServer::Instance()->UnregisterDebugTimer(this);

    this->critSect.Enter();
    this->history.Reset();
    this->timer.Reset();
    this->name.Clear();
    this->critSect.Leave();
}
        
//------------------------------------------------------------------------------
/**
*/
Timing::Time
DebugTimer::GetSample() const
{
    Timing::Time time = 0.0;
    this->critSect.Enter();
    if (!this->history.IsEmpty())
    {
        time = this->history.Back();
    }
    this->critSect.Leave();
    return time;
}

//------------------------------------------------------------------------------
/**
*/
Array<Timing::Time>
DebugTimer::GetHistory() const
{
    this->critSect.Enter();
    Array<Timing::Time> result = this->history.AsArray();
    this->critSect.Leave();
    return result;
}

} // namespace Debug
