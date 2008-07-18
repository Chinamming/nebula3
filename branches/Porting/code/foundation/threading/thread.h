#pragma once
#ifndef THREADING_THREAD_H
#define THREADING_THREAD_H
//------------------------------------------------------------------------------
/**
    @class Threading::Thread
    
    @todo describe Thread class
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "threading/win32/win32thread.h"
namespace Threading
{
class Thread : public Win32::Win32Thread
{ 
    DeclareClass(Thread);
};
}
#elif __XBOX360__
#include "threading/xbox360/xbox360thread.h"
namespace Threading
{
class Thread : public Xbox360::Xbox360Thread
{
    DeclareClass(Thread);
};
}
#elif __WII__
#include "threading/wii/wiithread.h"
namespace Threading
{
class Thread : public Wii::WiiThread
{
    DeclareClass(Thread);
};
}
#elif __APPLE__
#include "threading/posix/posixthread.h"
namespace Threading
{
class Thread : public Posix::PosixThread
{
    DeclareClass(Thread);
};
}
#else
#error "Threading::Thread not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
