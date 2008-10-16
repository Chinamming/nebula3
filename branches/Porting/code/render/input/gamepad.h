#pragma once
#ifndef INPUT_GAMEPAD_H
#define INPUT_GAMEPAD_H
//------------------------------------------------------------------------------
/**
    @class Input::GamePad
  
    An input handler which represents a game pad.
    
    (C) 2007 Radon Labs GmbH
*/    
#if (__WIN32__ || __XBOX360__)
#include "input/xinput/xinputgamepad.h"
namespace Input
{
class GamePad : public XInput::XInputGamePad
{
    __DeclareClass(GamePad);
};
}
#elif (__WII__)
#include "input/wii/wiigamepad.h"
namespace Input
{
class GamePad : public Wii::WiiGamePad
{
    __DeclareClass(GamePad);
};
}
#else
#error "GamePad class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
