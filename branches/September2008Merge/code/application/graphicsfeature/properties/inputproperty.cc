//------------------------------------------------------------------------------
//  properties/inputproperty.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/inputproperty.h"
#include "managers/focusmanager.h"
#include "game/entity.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/gamepad.h"
#include "graphicsprotocol.h"
#include "basegameprotocol.h"
#include "io/console.h"

namespace Attr
{
    DefineBoolWithDefault(InputFocus, 'IFOC', ReadWrite, true);
};

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeature::InputProperty, 'INPR', Game::Property);

using namespace Math;
using namespace Game;
using namespace Input;
using namespace BaseGameFeature;

//------------------------------------------------------------------------------
/**
*/
InputProperty::InputProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
InputProperty::~InputProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
InputProperty::SetupCallbacks()
{
    //this->entity->RegisterPropertyCallback(this, BeginFrame);
}

//------------------------------------------------------------------------------
/**
*/
void
InputProperty::SetupDefaultAttributes()
{
    Property::SetupDefaultAttributes();
    this->entity->AddBool(Attr::InputFocus);
}

//------------------------------------------------------------------------------
/**
*/
void
InputProperty::OnActivate()
{
    Property::OnActivate();
    if (this->entity->GetBool(Attr::InputFocus))
    {
        FocusManager::Instance()->SetInputFocusEntity(this->entity);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
InputProperty::OnDeactivate()
{
    // clear input focus, if we are the input focus object
    if (this->HasFocus())
    {
        FocusManager::Instance()->SetInputFocusEntity(0);
    }
    Property::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
InputProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(InputFocus::Id);    
    Game::Property::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void
InputProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg != 0);
    
    if (msg->CheckId(InputFocus::Id))
    {
        InputFocus* focusMsg = msg.downcast<InputFocus>();
        if (focusMsg->GetObtainFocus())
        {
            this->OnObtainFocus();
        }
        else
        {
            this->OnLoseFocus();
        }
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by the FocusManager when our entity gains the
    input focus. Override this method if your subclass needs to do 
    some initialization when gaining the input focus.
*/
void
InputProperty::OnObtainFocus()
{
    this->entity->SetBool(Attr::InputFocus, true);
}

//------------------------------------------------------------------------------
/**
    This method is called by the FocusManager when our entity loses
    the input focus. Override this method if your subclass needs to do any
    cleanup work here.
*/
void
InputProperty::OnLoseFocus()
{
    this->entity->SetBool(Attr::InputFocus, false);
}

//------------------------------------------------------------------------------
/**
    This method returns true if our entity has the input focus. This 
    implementation makes sure that 2 properties cannot report that they
    have the input focus by accident.
*/
bool
InputProperty::HasFocus() const
{
    return this->entity->GetBool(Attr::InputFocus);
}

//------------------------------------------------------------------------------
/**
    Handle general input.
*/
void
InputProperty::OnBeginFrame()
{/*
#if __DEBUG_ACTIVE__
    // only do something if we have the input focus
    if (FocusManager::Instance()->GetInputFocusEntity() == this->entity)
    {
        InputServer* inputServer = InputServer::Instance();
        
    #if !__WII__        
        if (inputServer->GetDefaultKeyboard()->KeyDown(Key::Space))
        {
            // send camera reset msg
            Ptr<CameraReset> camReset = CameraReset::Create();
            this->entity->SendSync(camReset.upcast<Messaging::Message>());
        }                
        if (inputServer->GetDefaultMouse()->ButtonPressed(MouseButton::RightButton))
        {
            float2 mouseMove = inputServer->GetDefaultMouse()->GetMovement();     

            // create CameraOrbit message
            Ptr<CameraOrbit> msg = CameraOrbit::Create();
            msg->SetHorizontalRotation(mouseMove.x());
            msg->SetVerticalRotation(mouseMove.y());
            this->entity->SendSync(msg.upcast<Messaging::Message>());            
        }
        if (inputServer->GetDefaultMouse()->WheelForward())
        {
            this->OnCameraZoomIn();
        }
        if (inputServer->GetDefaultMouse()->WheelBackward())
        {
            this->OnCameraZoomOut();
        }

        bool movement = false;
        if (inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::W))
        {            
            Ptr<MoveDirection> msg = MoveDirection::Create();
            msg->SetDirection(vector(0,0,1));
            msg->SetCameraRelative(true);
            this->entity->SendSync(msg.upcast<Messaging::Message>());                

            movement = true;
        }
        if (inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::S))
        {
            Ptr<MoveDirection> msg = MoveDirection::Create();
            msg->SetDirection(vector(0,0,1));
            msg->SetCameraRelative(true);
            this->entity->SendSync(msg.upcast<Messaging::Message>());                

            movement = true;
        }
        if (inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::W))
        {            
            Ptr<MoveDirection> msg = MoveDirection::Create();
            msg->SetDirection(vector(0,0,-1));
            msg->SetCameraRelative(true);
            this->entity->SendSync(msg.upcast<Messaging::Message>());                

            movement = true;
        }

        if (inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::A))
        {
            Ptr<MoveDirection> msg = MoveDirection::Create();
            msg->SetDirection(vector(-1,0,0));
            msg->SetCameraRelative(true);
            this->entity->SendSync(msg.upcast<Messaging::Message>());                

            movement = true;
        }
        if (inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::D))
        {
            Ptr<MoveDirection> msg = MoveDirection::Create();
            msg->SetDirection(vector(1,0,0));
            msg->SetCameraRelative(true);
            this->entity->SendSync(msg.upcast<Messaging::Message>());                

            movement = true;
        }        
        if (!movement)
        {
            Ptr<MoveStop> msg = MoveStop::Create();
            this->entity->SendSync(msg.upcast<Messaging::Message>());
        }
        
     #else   
     
        const Ptr<GamePad>& gamepad = inputServer->GetDefaultGamePad(0);
        if (gamepad->ButtonDown(GamePad::StartButton))
        {
            // send camera reset msg
            Ptr<CameraReset> camReset = CameraReset::Create();
            this->entity->SendSync(camReset.upcast<Messaging::Message>());
        }                
        if (gamepad->GetAxisValue(GamePad::LeftThumbXAxis) != 0
        ||  gamepad->GetAxisValue(GamePad::LeftThumbYAxis) != 0)
        {
            // create CameraOrbit message
            Ptr<CameraOrbit> msg = CameraOrbit::Create();
            msg->SetHorizontalRotation(gamepad->GetAxisValue(GamePad::LeftThumbXAxis) * 0.1f);
            msg->SetVerticalRotation(gamepad->GetAxisValue(GamePad::LeftThumbYAxis) * 0.1f);
            this->entity->SendSync(msg.upcast<Messaging::Message>());            
        }
        if (gamepad->GetAxisValue(GamePad::LeftTriggerAxis) != 0)
        {
            this->OnCameraZoomIn();
        }
        if (gamepad->GetAxisValue(GamePad::RightTriggerAxis) != 0)
        {
            this->OnCameraZoomOut();
        }
        if (gamepad->GetAxisValue(GamePad::RightThumbXAxis) != 0
            ||  gamepad->GetAxisValue(GamePad::RightThumbYAxis) != 0)
        {            
            Ptr<MoveDirection> msg = MoveDirection::Create();
            msg->SetDirection(vector(gamepad->GetAxisValue(GamePad::RightThumbXAxis),0,gamepad->GetAxisValue(GamePad::RightThumbYAxis)* -1.0f));
            msg->SetCameraRelative(true);
            this->entity->SendSync(msg.upcast<Messaging::Message>());                
        }       
        else
        {
            Ptr<MoveStop> msg = MoveStop::Create();
            this->entity->SendSync(msg.upcast<Messaging::Message>());
        }
    #endif
    }
#endif*/
}

//------------------------------------------------------------------------------
/**
    This sends a CameraDistance message if the user zooms in with the
    mouse wheel.

void
InputProperty::OnCameraZoomIn()
{
	Ptr<CameraDistance> msg = CameraDistance::Create();
    msg->SetRelativeDistanceChange(-1.0f);
	this->entity->SendSync(msg.upcast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
    This sends a CameraDistance message if the user zooms out with the
    mouse wheel.

void
InputProperty::OnCameraZoomOut()
{
	Ptr<CameraDistance> msg = CameraDistance::Create();
    msg->SetRelativeDistanceChange(+1.0f);
    this->entity->SendSync(msg.upcast<Messaging::Message>());
}
*/
}; // namespace GraphicsFeature
