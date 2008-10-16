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
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace XInput
{
class XInputGamePad : public Base::GamePadBase
{
    __DeclareClass(XInputGamePad);
public:
    /// constructor
    XInputGamePad();
    /// destructor
    virtual ~XInputGamePad();

protected:
    /// called when the handler is attached to the input server
    virtual void OnAttach();
    /// called on InputServer::BeginFrame()
    virtual void OnBeginFrame();
    /// update the state of a game pad button
    void UpdateButtonState(const XINPUT_GAMEPAD& curState, WORD xiBtn, Button btn);
    /// update the state of a trigger axis
    void UpdateTriggerAxis(const XINPUT_GAMEPAD& curState, Axis axis);
    /// update the state of a thumb stick axis
    void UpdateThumbAxis(const XINPUT_GAMEPAD& curState, Axis axis);

    static const Timing::Tick CheckConnectedInterval = 500;     // 500 ms between connection checks
    DWORD lastPacketNumber;
    Timing::Timer timer;
    Timing::Tick lastCheckConnectedTime;
};

} // namespace XInput
//------------------------------------------------------------------------------
#endif
