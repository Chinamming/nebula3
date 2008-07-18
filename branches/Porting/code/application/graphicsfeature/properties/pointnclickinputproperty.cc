//------------------------------------------------------------------------------
//  properties/pointnclickinputproperty.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/pointnclickinputproperty.h"
#include "properties/actorgraphicsproperty.h"

#include "game/entity.h"

#include "managers/focusmanager.h"
#include "managers/envquerymanager.h"

#include "input/inputserver.h"
#include "input/mouse.h"

#include "msg/movegoto.h"
#include "msg/movestop.h"
#include "msg/cameraorbit.h"
#include "msg/cameradistance.h"

#include "basegametiming/inputtimesource.h"


namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::PointNClickInputProperty, 'PACP', GraphicsFeature::InputProperty);

//using namespace Game;
using namespace BaseGameFeature;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
PointNClickInputProperty::PointNClickInputProperty() :
    moveGotoTime(0.0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
PointNClickInputProperty::~PointNClickInputProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
PointNClickInputProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, BeginFrame);
}

//------------------------------------------------------------------------------
/**
*/
void
PointNClickInputProperty::OnBeginFrame()
{
    // only do something if we have the input focus
    if (FocusManager::Instance()->GetInputFocusEntity() == GetEntity())
    {
        InputServer* inputServer = InputServer::Instance();
        if (inputServer->GetDefaultMouse()->ButtonDown(MouseButton::LeftButton))
        {
            this->OnLmbDown();
        }
        else if (inputServer->GetDefaultMouse()->ButtonPressed(MouseButton::LeftButton))
        {
            this->OnLmbPressed();
        }
        if (inputServer->GetDefaultMouse()->ButtonPressed(MouseButton::MiddleButton))
        {
            this->OnMmbPressed();
        }
        if (inputServer->GetDefaultMouse()->ButtonDown(MouseButton::RightButton))
        {
            inputServer->GetDefaultMouse()->BeginCapture();
            this->OnRmbDown();
        }
        if (inputServer->GetDefaultMouse()->ButtonUp(MouseButton::RightButton))
        {
            inputServer->GetDefaultMouse()->EndCapture();
        }
        if (inputServer->GetDefaultMouse()->WheelForward())
        {
            this->OnCameraZoomIn();
        }
        if (inputServer->GetDefaultMouse()->WheelBackward())
        {
            this->OnCameraZoomOut();
        }
    }
    InputProperty::OnBeginFrame();
}

//------------------------------------------------------------------------------
/**
    This sends a move goto message to the intersection point with the world.
*/
void
PointNClickInputProperty::SendMoveGoto()
{
    EnvQueryManager* envQueryMgr = EnvQueryManager::Instance();
    if (envQueryMgr->HasMouseIntersection())
    {
        // send a MoveGoto to the mouse intersection point
        const Math::point& mousePos = envQueryMgr->GetMousePos3d();
        const Ptr<BaseGameFeature::MoveGoto> msg = BaseGameFeature::MoveGoto::Create();
        msg->SetPosition(mousePos);
        this->entity->SendSync(msg.upcast<Messaging::Message>());

        // record the current time for the HandleLMBPressed() method
        this->moveGotoTime = InputTimeSource::Instance()->GetTime();
    }
}

//------------------------------------------------------------------------------
/**
    Handle the default action. This sends usually a MoveGoto message
    to our entity.
*/
void
PointNClickInputProperty::OnLmbDown()
{
    this->SendMoveGoto();
}

//------------------------------------------------------------------------------
/**
    This handles the case when the left mouse button remains pressed.
    In this case, a MoveGoto message is sent every half a second or so, so
    that the entity follows the mouse.
*/
void
PointNClickInputProperty::OnLmbPressed()
{
    Timing::Time curTime = InputTimeSource::Instance()->GetTime();
    if ((curTime - this->moveGotoTime) > 0.25f)
    {
        this->SendMoveGoto();
    }
}

//------------------------------------------------------------------------------
/**
    Handle the camera orbit movement.
*/
void
PointNClickInputProperty::OnMmbPressed()
{
    // get horizontal and vertical mouse movement
    InputServer* inputServer = InputServer::Instance();
    const Math::float2& mouseMove = inputServer->GetDefaultMouse()->GetMovement();

    // create CameraOrbit message
	Ptr<GraphicsFeature::CameraOrbit> msg = GraphicsFeature::CameraOrbit::Create();
    msg->SetHorizontalRotation(mouseMove.x());
    msg->SetVerticalRotation(mouseMove.y());
    this->entity->SendSync(msg.upcast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
    Handle a right mouse button click. This cancels the current movement.
*/
void
PointNClickInputProperty::OnRmbDown()
{
    // create a MoveStop message
	Ptr<BaseGameFeature::MoveStop> msg = BaseGameFeature::MoveStop::Create();
    this->entity->SendSync(msg.upcast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
    This sends a CameraDistance message if the user zooms in with the
    mouse wheel.
*/
void
PointNClickInputProperty::OnCameraZoomIn()
{
	Ptr<GraphicsFeature::CameraDistance> msg = GraphicsFeature::CameraDistance::Create();
    msg->SetRelativeDistanceChange(-1.0f);
	this->entity->SendSync(msg.upcast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
    This sends a CameraDistance message if the user zooms out with the
    mouse wheel.
*/
void
PointNClickInputProperty::OnCameraZoomOut()
{
	Ptr<GraphicsFeature::CameraDistance> msg = GraphicsFeature::CameraDistance::Create();
    msg->SetRelativeDistanceChange(+1.0f);
    this->entity->SendSync(msg.upcast<Messaging::Message>());
}


}; // namespace Properties