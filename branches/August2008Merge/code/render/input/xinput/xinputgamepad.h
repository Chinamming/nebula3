#pragma once
#ifndef XINPUT_XINPUTGAMEPAD_H
#define XINPUT_XINPUTGAMEPAD_H
//------------------------------------------------------------------------------
/**
    @class XInput::XInputGamePad
    
    Common gamepad support for Xbox360 and Windows.
    
    (C) 2007 Radon Labs GmbH
*/
#include "input/base/gamepadbase.h"

//------------------------------------------------------------------------------
namespace XInput
{
class XInputGamePad : public Base::GamePadBase
{
    DeclareClass(XInputGamePad);
public:
    /// constructor
    XInputGamePad();
    /// destructor
    virtual ~XInputGamePad();

protected:
    /// called on InputServer::BeginFrame()
    virtual void OnBeginFrame();
    /// update the state of a game pad button
    void UpdateButtonState(const XINPUT_GAMEPAD& curState, WORD xiBtn, Button btn);
    /// update the state of a trigger axis
    void UpdateTriggerAxis(const XINPUT_GAMEPAD& curState, Axis axis);
    /// update the state of a thumb stick axis
    void UpdateThumbAxis(const XINPUT_GAMEPAD& curState, Axis axis);

    DWORD lastPacketNumber;
};

} // namespace XInput
//------------------------------------------------------------------------------
#endif
