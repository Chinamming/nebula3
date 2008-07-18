//------------------------------------------------------------------------------
//  gamepad.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/gamepad.h"
#if (__WIN32__ || __XBOX360__)
namespace Input
{
ImplementClass(Input::GamePad, 'GMPD', XInput::XInputGamePad);
}
#elif (__WII__)
namespace Input
{
ImplementClass(Input::GamePad, 'GMPD', Wii::WiiGamePad);
}
#else
#error "GamePad class not implemented on this platform!"
#endif
