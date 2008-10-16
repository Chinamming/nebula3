//------------------------------------------------------------------------------
//  properties/mayacamerappoperty.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/mayacameraproperty.h"
#include "managers/focusmanager.h"
#include "graphics/cameraentity.h"
#include "game/entity.h"
#include "basegameprotocol.h"
#include "graphicsprotocol.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/inputserver.h"

// video camera specific attributes
namespace Attr
{
    DefineFloat4(MayaCameraCenterOfInterest, 'MCCI', ReadWrite);
	DefineFloat4WithDefault(MayaCameraDefaultUpVec, 'MCDU', ReadWrite, Math::float4(0.0f, 1.0f, 0.0f, 1.0f));
};

namespace GraphicsFeature
{

using namespace Input;
using namespace Math;

__ImplementClass(GraphicsFeature::MayaCameraProperty, 'MCAM', GraphicsFeature::CameraProperty);

//------------------------------------------------------------------------------
/**
*/
MayaCameraProperty::MayaCameraProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
MayaCameraProperty::~MayaCameraProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, Render);
}

//------------------------------------------------------------------------------
/**
    This adds the default attributes to the property.
*/
void
MayaCameraProperty::SetupDefaultAttributes()
{
    CameraProperty::SetupDefaultAttributes();
    this->entity->AddFloat4(Attr::MayaCameraCenterOfInterest);
    this->entity->AddFloat4(Attr::MayaCameraDefaultUpVec);
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::SetupAcceptedMessages()
{
	this->RegisterMessage(BaseGameFeature::UpdateTransform::Id);
	CameraProperty::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    if (msg->CheckId(BaseGameFeature::UpdateTransform::Id))
    {        
		this->cameraEntity->SetTransform(((BaseGameFeature::UpdateTransform*)msg.get())->GetMatrix());
    }
    else
    {
        CameraProperty::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::OnActivate()
{
    CameraProperty::OnActivate();

	// initialize the Maya camera object
	const matrix44& m = this->GetEntity()->GetMatrix44(Attr::Transform);
    
	// setup the camera util object
	this->mayaCameraUtil.Setup(	this->entity->GetFloat4(Attr::MayaCameraCenterOfInterest), 
								m.get_position(), 
								this->entity->GetFloat4(Attr::MayaCameraDefaultUpVec));
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::OnObtainFocus()
{
    CameraProperty::OnObtainFocus();
}

//------------------------------------------------------------------------------
/**
*/
void
MayaCameraProperty::OnRender()
{
	if (BaseGameFeature::FocusManager::Instance()->GetInputFocusEntity() == GetEntity())
    {
        // feed the Maya camera object with input
		InputServer* inputServer = InputServer::Instance();
		const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
		const Ptr<Mouse>& mouse = inputServer->GetDefaultMouse();
        const Ptr<GamePad>& gamePad = inputServer->GetDefaultGamePad(0);

		this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(MouseButton::LeftButton));
        this->mayaCameraUtil.SetPanButton(mouse->ButtonPressed(MouseButton::MiddleButton));
        this->mayaCameraUtil.SetZoomButton(mouse->ButtonPressed(MouseButton::RightButton));
        this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement());
        this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
        this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
     
		// TODO - gamepad support
		float zoomIn = 0.0f;
		float zoomOut = 0.0f;
		float2 panning(0.0f, 0.0f);
		float2 orbiting(0.0f, 0.0f);

		if (keyboard->KeyDown(Key::Space))
		{
			this->mayaCameraUtil.Reset();
		}
		if (keyboard->KeyPressed(Key::Left))
		{
			panning.x() -= 0.1f;
		}
		if (keyboard->KeyPressed(Key::Right))
		{
			panning.x() += 0.1f;
		}
		if (keyboard->KeyPressed(Key::Up))
		{
			panning.y() -= 0.1f;
		}
		if (keyboard->KeyPressed(Key::Down))
		{
			panning.y() += 0.1f;
		}

		this->mayaCameraUtil.SetPanning(panning);
		this->mayaCameraUtil.SetOrbiting(orbiting);
		this->mayaCameraUtil.SetZoomIn(zoomIn);
		this->mayaCameraUtil.SetZoomOut(zoomOut);
		this->mayaCameraUtil.Update();	
    }	

	// apply just the transform attribute (if we got focus)
	if (BaseGameFeature::FocusManager::Instance()->GetCameraFocusEntity() == GetEntity())
	{
		//if (!(GetEntity()->HasAttr(Attr::AnimPath) && GetEntity()->GetString(Attr::AnimPath).IsValid()))
		{
			// only use the internal matrix if we are not animated
			this->GetEntity()->SetMatrix44(Attr::Transform, this->mayaCameraUtil.GetCameraTransform());	
		}
	}

    // important: call parent class at the end to apply any further effects
    CameraProperty::OnRender();
}

}; // namespace Properties






