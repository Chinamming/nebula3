//------------------------------------------------------------------------------
//  xinputgamepad.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/xinput/xinputgamepad.h"

namespace XInput
{
ImplementClass(XInput::XInputGamePad, 'XIGP', Base::GamePadBase);

//------------------------------------------------------------------------------
/**
*/
XInputGamePad::XInputGamePad() :
    lastPacketNumber(0xffffffff)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
XInputGamePad::~XInputGamePad()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This compares the current state of the game pad against the
    previous state and sets the internal state accordingly.
*/
void
XInputGamePad::OnBeginFrame()
{
    GamePadBase::OnBeginFrame();

    // get current state of game pad
    XINPUT_STATE curState;
    DWORD result = XInputGetState(this->playerIndex, &curState);
    if (ERROR_SUCCESS == result)
    {
        this->isConnected = true;

        // check if state of controller has actually changed
        if (curState.dwPacketNumber != this->lastPacketNumber)
        {
            this->lastPacketNumber = curState.dwPacketNumber;

            // update button states
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_DPAD_UP, DPadUpButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_DPAD_DOWN, DPadDownButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_DPAD_LEFT, DPadLeftButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_DPAD_RIGHT, DPadRightButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_START, StartButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_BACK, BackButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_LEFT_THUMB, LeftThumbButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_RIGHT_THUMB, RightThumbButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_LEFT_SHOULDER, LeftShoulderButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_RIGHT_SHOULDER, RightShoulderButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_A, AButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_B, BButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_X, XButton);
            this->UpdateButtonState(curState.Gamepad, XINPUT_GAMEPAD_Y, YButton);

            // update axis states
            this->UpdateTriggerAxis(curState.Gamepad, LeftTriggerAxis);
            this->UpdateTriggerAxis(curState.Gamepad, RightTriggerAxis);
            this->UpdateThumbAxis(curState.Gamepad, LeftThumbXAxis);
            this->UpdateThumbAxis(curState.Gamepad, LeftThumbYAxis);
            this->UpdateThumbAxis(curState.Gamepad, RightThumbXAxis);
            this->UpdateThumbAxis(curState.Gamepad, RightThumbYAxis);
        }
    }
    else
    {
        // need to reset if we have been connected
        if (this->isConnected)
        {
            this->OnReset();
            this->isConnected = false;
        }
    }

    // if the vibrator settings have changed, update accordingly
    if (this->isConnected && this->vibratorsDirty)
    {
        XINPUT_VIBRATION vib;
        vib.wLeftMotorSpeed  = (WORD) (this->lowFreqVibrator * 65535.0f);
        vib.wRightMotorSpeed = (WORD) (this->highFreqVibrator * 65535.0f);
        XInputSetState(this->playerIndex, &vib);
        this->vibratorsDirty = false;
    }
}

//------------------------------------------------------------------------------
/**
    Compares the previous and current state of a game pad button
    and updates the parent class' state accordingly.
*/
void
XInputGamePad::UpdateButtonState(const XINPUT_GAMEPAD& curState, WORD xiBtn, Button btn)
{
    if (0 != (curState.wButtons & xiBtn))
    {
        // has button been down-pressed in this frame?
        if (!this->buttonStates[btn].pressed)
        {
            this->buttonStates[btn].down = true;
        }
        else
        {
            this->buttonStates[btn].down = false;
        }
        this->buttonStates[btn].pressed = true;
        this->buttonStates[btn].up = false;
    }
    else
    {
        // has button been released in this frame?
        if (this->buttonStates[btn].pressed)
        {
            this->buttonStates[btn].up = true;
        }
        else
        {
            this->buttonStates[btn].up = false;
        }
        this->buttonStates[btn].pressed = false;
        this->buttonStates[btn].down = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
XInputGamePad::UpdateTriggerAxis(const XINPUT_GAMEPAD& curState, Axis axis)
{
    n_assert((axis == LeftTriggerAxis) || (axis == RightTriggerAxis));

    BYTE rawValue = 0;
    if (LeftTriggerAxis == axis)
    {
        rawValue = curState.bLeftTrigger;
    }
    else
    {
        rawValue = curState.bRightTrigger;
    }
    this->axisValues[axis] = n_min(0.0f, float(rawValue - XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) / (255.0f - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

//------------------------------------------------------------------------------
/**
*/
void
XInputGamePad::UpdateThumbAxis(const XINPUT_GAMEPAD& curState, Axis axis)
{
    n_assert((axis == LeftThumbXAxis) || (axis == LeftThumbYAxis) || (axis == RightThumbXAxis) || (axis == RightThumbYAxis));
    
    SHORT rawValue = 0;
    SHORT deadZone = 0;
    switch (axis)
    {
        case LeftThumbXAxis:
            rawValue = curState.sThumbLX;
            deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            break;

        case LeftThumbYAxis:
            rawValue = curState.sThumbLY;
            deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            break;

        case RightThumbXAxis:
            rawValue = curState.sThumbRX;
            deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
            break;

        case RightThumbYAxis:
            rawValue = curState.sThumbRY;
            deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
            break;
    }
    float val = 0.0f;
    if ((rawValue > deadZone) || (rawValue < -deadZone))
    {
        // outside dead zone, scale from -1.0f to +1.0f
        if (rawValue > 0)
        {
            // in positive range
            val = n_max(0.0f, float(rawValue - deadZone)) / (32767.0f - deadZone);            
        }
        else
        {
            // in negative range
            val = n_min(0.0f, float(rawValue + deadZone)) / (32768.0f - deadZone);            
        }
    }
    this->axisValues[axis] = val;
}

} // namespace XInput
