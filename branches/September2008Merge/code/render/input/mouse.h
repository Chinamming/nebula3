#pragma once
#ifndef INPUT_MOUSE_H
#define INPUT_MOUSE_H
//------------------------------------------------------------------------------
/**
    @class Input::Mouse
    
    An input handler which represents a mouse for polling.

    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "input/win32/win32mouse.h"
namespace Input
{
class Mouse : public Win32::Win32Mouse
{
    __DeclareClass(Mouse);
};
}
#elif __XBOX360__
#include "input/xbox360/xbox360mouse.h"
namespace Input
{
class Mouse : public Xbox360::Xbox360Mouse
{
    __DeclareClass(Mouse);
};
}
#elif __WII__
#include "input/wii/wiimouse.h"
namespace Input
{
class Mouse : public Wii::WiiMouse
{
    __DeclareClass(Mouse);
};
}
#else
#error "Mouse class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif

