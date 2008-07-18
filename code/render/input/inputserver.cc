//------------------------------------------------------------------------------
//  inputserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/inputserver.h"

namespace Input
{
#if __WIN32__
ImplementClass(Input::InputServer, 'INPS', Win32::Win32InputServer);
ImplementSingleton(Input::InputServer);
#elif __XBOX360__
// FIXME!
ImplementClass(Input::InputServer, 'INPS', Xbox360::Xbox360InputServer);
ImplementSingleton(Input::InputServer);
#elif __WII__
ImplementClass(Input::InputServer, 'INPS', Wii::WiiInputServer);
ImplementSingleton(Input::InputServer);
#else
#error "InputServer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
InputServer::InputServer()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
InputServer::~InputServer()
{
    DestructSingleton;
}

} // namespace Input
