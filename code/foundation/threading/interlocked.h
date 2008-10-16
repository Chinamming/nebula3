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
#if (__WIN32__ || __XBOX360__)
#include "threading/win360/win360interlocked.h"
namespace Threading
{
class Interlocked : public Win360::Win360Interlocked
{ };
}
#elif __WII__
#include "threading/wii/wiiinterlocked.h"
namespace Threading
{
class Interlocked : public Wii::WiiInterlocked
{ };
}
#elif __APPLE__
#include "threading/darwin/darwininterlocked.h"
namespace Threading
{
class Interlocked : public Darwin::DarwinInterlocked
{ };
}
#elif __unix__
#include "threading/posix/posixinterlocked.h"
namespace Threading
{
class Interlocked : public Posix::PosixInterlocked
{ };
}
#else
#error "Threading::Interlocked not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
