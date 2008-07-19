#pragma once
#ifndef THREADING_EVENT_H
#define THREADING_EVENT_H
//------------------------------------------------------------------------------
/**
    @class Threading::Event
  
    @todo describe Event class
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "threading/win32/win32event.h"
namespace Threading
{
class Event : public Win32::Win32Event
{ };
}
#elif __XBOX360__
#include "threading/xbox360/xbox360event.h"
namespace Threading
{
class Event : public Xbox360::Xbox360Event
{ };
}
#elif __WII__
#include "threading/wii/wiievent.h"
namespace Threading
{
class Event : public Wii::WiiEvent
{ };
}
#else
#error "Threading::Event not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
