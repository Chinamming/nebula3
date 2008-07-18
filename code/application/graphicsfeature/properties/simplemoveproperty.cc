//------------------------------------------------------------------------------
//  properties/simplemoveproperty.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/simplemoveproperty.h"
#include "core/factory.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "basegametiming/gametimesource.h"
#include "math/polar.h"
#include "graphics/graphicsserver.h"
#include "graphics/cameraentity.h"
#include "managers/entitymanager.h"
#include "basegametiming/inputtimesource.h"
#include "msg/movesetvelocity.h"

namespace Attr
{
    DefineFloatWithDefault(Velocity, 'VELO', ReadWrite, 100.0f);
};

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::SimpleMoveProperty, 'SMPR', TransformableProperty);

using namespace Game;
using namespace Messaging;
using namespace Timing;
using namespace Math;
using namespace Util;
using namespace BaseGameFeature;

//------------------------------------------------------------------------------
/**
*/
SimpleMoveProperty::SimpleMoveProperty() :
    headingGain(-20.0f),
    positionGain(-25.0f),
    curSpeed(0),
    moveDir(0,0,0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SimpleMoveProperty::~SimpleMoveProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
SimpleMoveProperty::SetupDefaultAttributes()
{
    TransformableProperty::SetupDefaultAttributes();
    this->entity->AddFloat(Attr::Velocity);
}

//------------------------------------------------------------------------------
/**
*/
void
SimpleMoveProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, MoveAfter);
}

//------------------------------------------------------------------------------
/**
*/
void
SimpleMoveProperty::OnActivate()
{
    TransformableProperty::OnActivate();
    
    // reset the feedback loops 
    Time time = GameTimeSource::Instance()->GetTime();
    this->smoothedPosition.Reset(time, 0.001f, this->positionGain, entity->GetMatrix44(Attr::Transform).getpos_component());
    polar headingAngle(entity->GetMatrix44(Attr::Transform).getz_component());
    this->smoothedHeading.Reset(time, 0.001f, this->headingGain, headingAngle.rho);
    
    this->curSpeed = 0.0f;
}

//------------------------------------------------------------------------------
/**
*/
void
SimpleMoveProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(MoveDirection::Id);
    this->RegisterMessage(MoveStop::Id);
    this->RegisterMessage(SetTransform::Id);
    this->RegisterMessage(MoveTurn::Id);
    this->RegisterMessage(MoveSetVelocity::Id);
    this->RegisterMessage(MoveRotate::Id);
    TransformableProperty::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleMoveProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    if (msg->CheckId(MoveDirection::Id))
    {
        this->HandleMoveDirection((MoveDirection*) msg.get());
    }
    else if (msg->CheckId(MoveStop::Id))
    {
        this->Stop();
    }
    else if (msg->CheckId(MoveTurn::Id))
    {
        this->HandleMoveTurn((MoveTurn*) msg.get());
    }
    else if (msg->CheckId(MoveRotate::Id))
    {
        this->HandleMoveRotate((MoveRotate*) msg.get());
    }
    else if (msg->CheckId(MoveSetVelocity::Id))
    {
        this->GetEntity()->SetFloat(Attr::Velocity, ((MoveSetVelocity*)msg.get())->GetRelVelocity());
    }
    else
    {
        TransformableProperty::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
    The OnMoveAfter() method transfers the current physics entity transform to 
    the game entity.
*/
void
SimpleMoveProperty::OnMoveAfter()
{
    if (this->curSpeed > 0.0)
    {
        const matrix44& curTransform = entity->GetMatrix44(Attr::Transform);
    
        Time frameTime = GameTimeSource::Instance()->GetFrameTime();
        // feed the feedback loops
        polar headingAngles(-this->moveDir);
        this->smoothedPosition.SetGoal(curTransform.getpos_component() + this->moveDir * this->curSpeed * (float)frameTime);   
        this->smoothedHeading.SetGoal(headingAngles.rho);

        // if current state and goal are already close to each other,
        // we don't send a position update
        // NOTE: this check must be made before the feedback look update!
        vector posError = this->smoothedPosition.GetState() - this->smoothedPosition.GetGoal();
        float headingError = n_abs(this->smoothedHeading.GetState() - this->smoothedHeading.GetGoal());

        // always evaluate the feedback loops to keep them in valid time delta
        Time time = GameTimeSource::Instance()->GetTime();
        this->smoothedPosition.Update(time);
        this->smoothedHeading.Update(time);

        // only send update transform if anything changed
        //if ((posError.length() > 0.001f) || (headingError > 0.001f))
        {
            // construct the new entity matrix
            matrix44 entityMatrix = matrix44::rotationy(this->smoothedHeading.GetState());
            entityMatrix.translate(this->smoothedPosition.GetState());

            // update game entity
            Ptr<UpdateTransform> msg = UpdateTransform::Create();
            msg->SetMatrix(entityMatrix);
            this->GetEntity()->SendSync(msg.upcast<Message>());
        }
    }
    else
    {
        Time time = GameTimeSource::Instance()->GetTime();
        this->smoothedPosition.Reset(time, 0.001f, this->positionGain, entity->GetMatrix44(Attr::Transform).getpos_component());
        polar headingAngle(entity->GetMatrix44(Attr::Transform).getz_component());
        this->smoothedHeading.Reset(time, 0.001f, this->headingGain, headingAngle.rho);    
    }
}

//------------------------------------------------------------------------------
/**
    This simply sends a synchronous stop message to myself. This uses
    a message so that everybody else who might be interested in the information
    that I have stopped can listen to the message.
*/
void
SimpleMoveProperty::SendStop()
{
    Ptr<MoveStop> msg = MoveStop::Create();
    this->GetEntity()->SendSync(msg.upcast<Message>());
}

//------------------------------------------------------------------------------
/**    
*/
void
SimpleMoveProperty::Stop()
{    
    this->curSpeed = 0.0f;
}

//------------------------------------------------------------------------------
/**
    Handle a MoveDirection message.
*/
void
SimpleMoveProperty::HandleMoveDirection(MoveDirection* msg)
{
    n_assert(msg);

    vector dir = msg->GetDirection();
    // convert camera relative vector into absolute vector if necessary
    if (msg->GetCameraRelative())
    {
        const Ptr<Graphics::View>& curView = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultView();
        Graphics::CameraEntity* camera = curView->GetCameraEntity();
        n_assert(camera);
        matrix44 camTransform = camera->GetTransform();
        camTransform.setpos_component(float4(0.0f, 0.0f, 0.0f, 1.0f));
        dir = vector::transform(dir, camTransform);
    }
    dir.y() = 0.0f;
    //dir = vector::normalize(dir);

    this->curSpeed = this->GetEntity()->GetFloat(Attr::Velocity);
    this->moveDir = dir;
}

//------------------------------------------------------------------------------
/**
    Handle a MoveTurn message.
*/
void
SimpleMoveProperty::HandleMoveTurn(MoveTurn* msg)
{
    n_assert(msg);
    vector dir = msg->GetDirection();
    if (msg->GetCameraRelative())
    {
        const Ptr<Graphics::View>& curView = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultView();
        Graphics::CameraEntity* camera = curView->GetCameraEntity();
        n_assert(camera);
        matrix44 camTransform = camera->GetTransform();
        camTransform.setpos_component(float4(0.0f, 0.0f, 0.0f, 1.0f));
        dir = vector::transform(dir, camTransform);
    }
    dir.y() = 0.0f;
    dir = vector::normalize(dir);
    this->moveDir = dir;
}

//------------------------------------------------------------------------------
/**
    Handle a MoveTurn message.
*/
void
SimpleMoveProperty::HandleMoveRotate(MoveRotate* msg)
{
    n_assert(msg);
    
    Time frameTime = InputTimeSource::Instance()->GetFrameTime();
    float angle = msg->GetAngle() * (float)frameTime;
    const matrix44& curTransform = entity->GetMatrix44(Attr::Transform);
    vector direction = vector::transform(-curTransform.getz_component(), matrix44::rotationy(angle));
    
    this->moveDir = direction;
}

} // namespace Properties
