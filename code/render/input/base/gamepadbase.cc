//------------------------------------------------------------------------------
//  gamepadbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/base/gamepadbase.h"

namespace Base
{
ImplementClass(Base::GamePadBase, 'IGPB', Input::InputHandler);

using namespace Input;

//------------------------------------------------------------------------------
/**
*/
GamePadBase::GamePadBase() :
    playerIndex(0),
    isConnected(false),
    buttonStates(NumButtons),
    axisValues(NumAxes, 0.0f),
    vibratorsDirty(false),
    lowFreqVibrator(0.0f),
    highFreqVibrator(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GamePadBase::~GamePadBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
GamePadBase::OnAttach()
{
    InputHandler::OnAttach();

    // need to reset our state
    ButtonState initialButtonState;
    this->buttonStates.Clear(initialButtonState);
    this->axisValues.Clear(0.0f);
}

//------------------------------------------------------------------------------
/**
*/
void
GamePadBase::OnReset()
{
    InputHandler::OnReset();

    // reset button states (should behave as if the player
    // suddenly released all buttons and triggers)
    IndexT i;
    for (i = 0; i < this->buttonStates.Size(); i++)
    {
        ButtonState& btnState = this->buttonStates[i];
        if (btnState.pressed)
        {
            btnState.up = true;
        }
        else
        {
            btnState.up = false;
        }
        btnState.down = false;
        btnState.pressed = false;
    }
    this->axisValues.Clear(0.0f);
}

} // namespace Base
