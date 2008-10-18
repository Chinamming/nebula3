//------------------------------------------------------------------------------
//  properties/cameraproperty.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/cameraproperty.h"
#include "managers/focusmanager.h"
#include "game/entity.h"
#include "graphics/graphicsserver.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "graphics/cameraentity.h"
#include "graphics/view.h"
#include "graphics/stage.h"
#include "graphicsprotocol.h"
#include "basegamefeature/basegameattr/basegameattributes.h"

// camera specific attributes
namespace Attr
{
    DefineBoolWithDefault(CameraFocus, 'CAMF', ReadWrite, true);
};

namespace GraphicsFeature
{
__ImplementClass(GraphicsFeature::CameraProperty, 'CAMP', Game::Property);

using namespace Game;
using namespace Math;
using namespace BaseGameFeature;

//------------------------------------------------------------------------------
/**
*/
CameraProperty::CameraProperty()
{
    this->cameraEntity = Graphics::CameraEntity::Create();
}

//------------------------------------------------------------------------------
/**
*/
CameraProperty::~CameraProperty()
{
    this->cameraEntity = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
CameraProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, Render);
}

//------------------------------------------------------------------------------
/**
*/
void
CameraProperty::SetupDefaultAttributes()
{
    Property::SetupDefaultAttributes();
    this->entity->AddBool(Attr::CameraFocus);
}

//------------------------------------------------------------------------------
/**
*/
void
CameraProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(CameraFocus::Id);    
    Game::Property::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void
CameraProperty::OnStart()
{
    Property::OnStart();

    if (this->entity->GetBool(Attr::CameraFocus))
    {
        FocusManager::Instance()->SetCameraFocusEntity(this->entity);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CameraProperty::OnDeactivate()
{
    // clear camera focus, if we are the camera focus object
    if (this->HasFocus())
    {
        FocusManager::Instance()->SetCameraFocusEntity(0);
    }
    Property::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
CameraProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg != 0);
    
    if (msg->CheckId(CameraFocus::Id))
    {
        const Ptr<CameraFocus>& focusMsg = msg.downcast<CameraFocus>();
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
    camera focus. Override this method if your subclass needs to do 
    some initialization when gaining the camera focus.
*/
void
CameraProperty::OnObtainFocus()
{
    // update focus attribute
    this->entity->SetBool(Attr::CameraFocus, true);
    
    this->defaultStage = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultStage();
    this->defaultView = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultView();
    this->defaultStage->AttachEntity(this->cameraEntity.cast<Graphics::GraphicsEntity>());
    this->defaultView->SetCameraEntity(this->cameraEntity);
}

//------------------------------------------------------------------------------
/**
    This method is called by the FocusManager when our entity loses
    the camera focus. Override this method if your subclass needs to do any
    cleanup work here.
*/
void
CameraProperty::OnLoseFocus()
{
    // clear cam in default view
    if (this->defaultView->GetCameraEntity() == this->cameraEntity)
    {
        this->defaultView->SetCameraEntity(0);
    }
    this->defaultStage->RemoveEntity(this->cameraEntity.cast<Graphics::GraphicsEntity>());
    this->defaultStage = 0;
    this->defaultView = 0;

    // update focus attribute
    this->entity->SetBool(Attr::CameraFocus, false);
}

//------------------------------------------------------------------------------
/**
    This method returns true if our entity has the camera focus. This 
    implementation makes sure that 2 properties cannot report that they
    have the camera focus by accident.
*/
bool
CameraProperty::HasFocus() const
{
    return this->entity->GetBool(Attr::CameraFocus);
}

//------------------------------------------------------------------------------
/**
    Called before camera is "rendered". The default camera properties
    applies shake effects to the camera.
*/
void
CameraProperty::OnRender()
{
	// do only if we got focus
    if (FocusManager::Instance()->GetCameraFocusEntity() == this->entity)
    {        
		// apply final transform
        n_assert(this->cameraEntity != 0);
		this->cameraEntity->SetTransform(this->GetEntity()->GetMatrix44(Attr::Transform));
        
		/* ========================================================================================= NEB 2 STUFF 
		TODO!?!?! shaker effect stuff 

		this->shakeEffectHelper.SetCameraTransform(camera->GetTransform());
        this->shakeEffectHelper.Update();
        camera->SetTransform(this->shakeEffectHelper.GetShakeCameraTransform());
        
        // if enity has transform set the current position between camera and entity as audio listener position
        // otherwise only use camera transform

        // TODO: use new audio listener
        Audio::Listener* listener = Audio::Server::Instance()->GetListener();
        n_assert(listener);
        matrix44 transform = camera->GetTransform();
        if (this->entity->HasAttr(Attr::Transform))
        {
            const matrix44& enityTransform = this->entity->GetMatrix44(Attr::Transform);
            transform.translate((enityTransform.pos_component() - transform.pos_component()) * 0.5f);
        }
        listener->SetTransform(transform);

        // TODO: set the point-of-interest in the posteffects subsystem
        PostEffects::Server::Instance()->SetPointOfInterest(this->entity->GetMatrix44(Attr::Transform).pos_component());
        =========================================================================================== NEB 2 STUFF */
		
    }
}

}; // namespace GraphicsFeature
