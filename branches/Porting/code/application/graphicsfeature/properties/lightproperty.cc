//------------------------------------------------------------------------------
//  properties/lightproperty.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/lightproperty.h"
#include "core/factory.h"
#include "game/entity.h"
#include "basegameattr/basegameattributes.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "msg/updatetransform.h"
//#include "msg/attributesupdated.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "lighting/spotlightentity.h"
#include "lighting/globallightentity.h"

namespace Attr
{
    DefineIntWithDefault(LightType, 'LITY', ReadOnly, true);
    DefineFloat4WithDefault(LightColor, 'LCOL', ReadWrite, Math::float4(1.0f, 1.0f, 1.0f, 1.0f));
    DefineFloat4WithDefault(LightOppositeColor, 'LOCL', ReadWrite, Math::float4(0.1f, 0.1f, 0.2f, 1.0f));
    DefineFloatWithDefault(LightIntensity, 'LINT', ReadWrite, 1.0f);
    DefineFloatWithDefault(LightRange, 'LRAN', ReadWrite, 10.0f);    
    DefineFloat4WithDefault(LightAmbient, 'LAMB', ReadWrite, Math::float4(0.1, 0.1f, 0.1f, 1.0f));
    DefineBoolWithDefault(LightCastShadows, 'LCAS', ReadWrite, true);    
    DefineBoolWithDefault(LightFlickerEnable, 'LFLE', ReadWrite, false);
    DefineFloatWithDefault(LightFlickerFrequency, 'LFLF', ReadWrite, 5.0f);
    DefineFloatWithDefault(LightFlickerIntensity, 'LFLI', ReadWrite, 0.5f);
    DefineFloatWithDefault(LightFlickerPosition, 'LFLP', ReadWrite, 0.25f);
};

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::LightProperty, 'LIPR', Game::Property);

using namespace Game;
using namespace Messaging;
using namespace Lighting;
using namespace Math;
using namespace BaseGameFeature;

bool LightProperty::globalLightExists = false;
//------------------------------------------------------------------------------
/**
*/
LightProperty::LightProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
LightProperty::~LightProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::SetupDefaultAttributes()
{
    Property::SetupDefaultAttributes();
    this->entity->AddMatrix44(Attr::Transform);
    this->entity->AddInt(Attr::LightType);
    this->entity->AddFloat4(Attr::LightColor);
    this->entity->AddFloat4(Attr::LightOppositeColor);
    this->entity->AddFloat(Attr::LightRange);
    this->entity->AddFloat4(Attr::LightAmbient);    
    this->entity->AddBool(Attr::LightCastShadows);
    this->entity->AddBool(Attr::LightFlickerEnable);
    this->entity->AddFloat(Attr::LightFlickerFrequency);
    this->entity->AddFloat(Attr::LightFlickerIntensity);
    this->entity->AddFloat(Attr::LightFlickerPosition);
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, Render);
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::OnActivate()
{
    Property::OnActivate();

    // create and setup graphics light entity
    // TODO: handle global light
    int lightCode = GetEntity()->GetInt(Attr::LightType);    
#if (1)//__NEBULA2_EXPORT__
    // map lightcode from n2 to n3
    // nebula2:          in nebula3:
    // Point = 0,        Global = 0,
    // Directional,      Spot,
    // Spot,             Point,

    int LightCodesSubstitute[2]; 
    LightCodesSubstitute[0] = Lighting::LightType::Spot; // no pointlights yet
    LightCodesSubstitute[1] = Lighting::LightType::Global;
    lightCode = LightCodesSubstitute[lightCode];
#endif

    switch ((Lighting::LightType::Code)lightCode)
    {
    case Lighting::LightType::Global:
        {
            // only one global light should exists at one time
            n_assert(!globalLightExists);
            Ptr<Lighting::GlobalLightEntity> globalLight = Lighting::GlobalLightEntity::Create();
            globalLight->SetBackLightColor(GetEntity()->GetFloat4(Attr::LightOppositeColor));
            this->lightEntity = globalLight;
            globalLightExists = true;
        }
        break;
    case Lighting::LightType::Spot:
        {
            this->lightEntity = Lighting::SpotLightEntity::Create();
        }
        break;
    case Lighting::LightType::Point:
        // no pointlights yet
        break;
    }	
    this->UpdateLightFromAttributes();    

    point lightPos(this->entity->GetMatrix44(Attr::Transform).getpos_component());
    vector lightDir(this->entity->GetMatrix44(Attr::Transform).getz_component());
    lightDir = float4::normalize(lightDir);
    point lookAt = lightPos + lightDir;
    float scale = n_max(GetEntity()->GetFloat(Attr::LightRange), 1.0f);
    matrix44 lightTransform = matrix44::multiply(matrix44::scaling(scale, scale, scale), matrix44::lookatrh(lightPos, lookAt, vector::upvec()));    
    this->lightEntity->SetTransform(lightTransform);

    // register with graphics server
    Graphics::Stage* stage = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultStage();
    n_assert(stage);
    stage->AttachEntity(this->lightEntity.upcast<Graphics::GraphicsEntity>());

    // setup the light flicker utility
    this->lightFlickerUtil.SetLightEntity(this->lightEntity);
    this->UpdateLightFlickerUtilFromAttributes();
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::OnDeactivate()
{
    if (this->lightEntity->IsA(Lighting::GlobalLightEntity::RTTI))
    {
        globalLightExists = false;
    }
    // cleanup graphics light property
    Graphics::Stage* stage = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultStage();
    stage->RemoveEntity(this->lightEntity.upcast<Graphics::GraphicsEntity>());
    this->lightEntity = 0;
    
    Property::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::OnRender()
{
    // need to update the light flickering utility
    this->lightFlickerUtil.Update();
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(UpdateTransform::Id);
    //this->RegisterMessage(AttributesUpdated::Id);
    Property::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**
*/
void
LightProperty::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg);
    if (msg->CheckId(UpdateTransform::Id))
    {
        UpdateTransform* updTransform = msg.downcast<UpdateTransform>();
        point lightPos(updTransform->GetMatrix().getpos_component());
        vector lightDir(updTransform->GetMatrix().getz_component());
        point lookAt = lightPos + lightDir;
        float scale = GetEntity()->GetFloat(Attr::LightRange);
        matrix44 lightTransform = matrix44::multiply(matrix44::scaling(scale, scale, scale), matrix44::lookatrh(lookAt, lightPos, vector::upvec()));
        this->lightEntity->SetTransform(lightTransform);    
    }
    //else if (msg->CheckId(AttributesUpdated::Id))
    //{
    //    // this is usually coming only from the level editor
    //    this->UpdateLightFlickerUtilFromAttributes();
    //    this->UpdateLightFromAttributes();
    //}
    else
    {
        Property::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
    Update the light flicker util object from attributes.
*/
void
LightProperty::UpdateLightFlickerUtilFromAttributes()
{
    this->lightFlickerUtil.SetEnabled(this->entity->GetBool(Attr::LightFlickerEnable));
    this->lightFlickerUtil.SetFrequency(this->entity->GetFloat(Attr::LightFlickerFrequency));
    this->lightFlickerUtil.SetIntensityAmplitude(this->entity->GetFloat(Attr::LightFlickerIntensity));
    this->lightFlickerUtil.SetPositionAmplitude(this->entity->GetFloat(Attr::LightFlickerPosition));
}

//------------------------------------------------------------------------------
/**
    Update embedded Nebula2 light object from attributes.
*/
void
LightProperty::UpdateLightFromAttributes()
{    
    this->lightEntity->SetColor(this->entity->GetFloat4(Attr::LightColor) * this->entity->GetFloat(Attr::LightIntensity));    
    this->lightEntity->SetCastShadows(this->entity->GetBool(Attr::LightCastShadows));        
}

}; // namespace GraphicsFeature
