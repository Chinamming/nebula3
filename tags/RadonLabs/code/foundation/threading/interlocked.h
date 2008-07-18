#pragma once
#ifndef THREADING_INTERLOCKED_H
#define THREADING_INTERLOCKED_H
//------------------------------------------------------------------------------
/**
    @class Threading::Interlocked
    
    Provide simple atomic operations on memory variables.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "threading/win32/win32interlocked.h"
namespace Threading
{
class Interlocked : public Win32::Win32Interlocked
{ };
}
#elif __XBOX360__
#include "threading/xbox360/xbox360interlocked.h"
namespace Threading
{
class Interlocked : public Xbox360::Xbox360Interlocked
{ };
}
#elif __WII__
#include "threading/wii/wiiinterlocked.h"
namespace Threading
{
class Interlocked : public Wii::WiiInterlocked
{ };
}
#else
#error "Threading::Interlocked not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
