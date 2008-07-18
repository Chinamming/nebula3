#pragma once
#ifndef THREADING_CRITICALSECTION_H
#define THREADING_CRITICALSECTION_H
//------------------------------------------------------------------------------
/**
    @class Threading::CriticalSection

    Critical section objects are used to protect a portion of code from parallel
    execution. Define a static critical section object and use its Enter() 
    and Leave() methods to protect critical sections of your code.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "threading/win32/win32criticalsection.h"
namespace Threading
{
class CriticalSection : public Win32::Win32CriticalSection
{ };
};
#elif __XBOX360__
#include "threading/xbox360/xbox360criticalsection.h"
namespace Threading
{
class CriticalSection : public Xbox360::Xbox360CriticalSection
{ };
}
#elif __WII__
#include "threading/wii/wiicriticalsection.h"
namespace Threading
{
class CriticalSection : public Wii::WiiCriticalSection
{ };
}
#elif __APPLE__
#include "threading/posix/posixcriticalsection.h"
namespace Threading
{
class CriticalSection : public Posix::PosixCriticalSection
{ };
}
#else
#error "Threading::CriticalSection not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
    
