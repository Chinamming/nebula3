//------------------------------------------------------------------------------
//  properties/graphicsproperty.h
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/graphicsproperty.h"
#include "appgame/appconfig.h"
#if __USE_PHYSICS__
//#include "graphicsutil/physicsgfxutil.h"
//#include "properties/physicsproperty.h"
#endif
#include "graphicsfeatureunit.h"
#include "graphicsutil/segmentedgfxutil.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "game/entity.h"
#include "basegameattr/basegameattributes.h"
#include "msg/updatetransform.h"
#include "msg/setvisible.h"
#include "msg/getgraphicsentities.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::GraphicsProperty, 'GFXP', Game::Property);

using namespace Game;
using namespace Math;
using namespace BaseGameFeature;

//------------------------------------------------------------------------------
/**
*/
GraphicsProperty::GraphicsProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GraphicsProperty::~GraphicsProperty()
{
    n_assert(this->graphicsEntities.Size() == 0);
}

//------------------------------------------------------------------------------
/** 
*/
void
GraphicsProperty::SetupDefaultAttributes()
{
    Property::SetupDefaultAttributes();
    this->entity->AddString(Attr::Graphics);
}

//------------------------------------------------------------------------------
/** 
    Attach the property to a game entity. This will create and setup
    the required graphics entities.
*/
void
GraphicsProperty::OnActivate()
{
    Game::Property::OnActivate();
    this->SetupGraphicsEntities();
}

//------------------------------------------------------------------------------
/**    
    Remove the property from its game entity. This will release the
    graphics entities owned by the property.
*/
void
GraphicsProperty::OnDeactivate()
{
    Graphics::Stage* stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();

    // release graphics entities
    int i;
    int num = this->graphicsEntities.Size();
    for (i = 0;  i < num; i++)
    {
        Graphics::GraphicsEntity* gfxEntity = this->graphicsEntities[i];
        if (gfxEntity->GetCell())
        {
            stage->RemoveEntity(gfxEntity);
        }
        this->graphicsEntities[i] = 0;
    }
    this->graphicsEntities.Clear();

    // up to parent class
    Property::OnDeactivate();
}

//------------------------------------------------------------------------------
/**    
    Setup the graphics entities. You may override this method in a subclass
    if different setup is needed.
*/
void
GraphicsProperty::SetupGraphicsEntities()
{
    // get some entity attributes
    Util::String resName =  this->GetGraphicsResource();
    Graphics::Stage* stage = Graphics::GraphicsServer::Instance()->GetStageByName(Util::StringAtom("DefaultStage"));

#if __USE_PHYSICS__
    //// check if we have a physics property attached
    //PhysicsFeature::PhysicsProperty* physProperty = (PhysicsFeature::PhysicsProperty*) GetEntity()->FindProperty(PhysicsFeature::PhysicsProperty::RTTI).get();
    //if (physProperty && physProperty->IsEnabled())
    //{
    //    // setup graphics entities for visualizing physics
    //    Physics::PhysicsEntity* physEntity = physProperty->GetPhysicsEntity();
    //    if (physEntity)
    //    {
    //        this->graphicsEntities = Util::PhysicsGfxUtil::CreateGraphics(physEntity);
    //        n_assert(this->graphicsEntities.Size() > 0);
    //        Util::PhysicsGfxUtil::SetupGraphics(resName, physEntity, this->graphicsEntities);
    //        return;
    //    }
    //}
#endif

    // fallthrough: setup physics-less graphics entity
    const matrix44 worldMatrix = GetEntity()->GetMatrix44(Attr::Transform);
    SegmentedGfxUtil segGfxUtil;
    this->graphicsEntities = segGfxUtil.CreateAndSetupGraphicsEntities(resName, worldMatrix, stage);
}

//------------------------------------------------------------------------------
/**    
*/
void
GraphicsProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(UpdateTransform::Id);
    this->RegisterMessage(SetVisibleMsg::Id);
    this->RegisterMessage(GetGraphicsEntities::Id);
    Property::SetupAcceptedMessages();
}

//------------------------------------------------------------------------------
/**    
*/
void
GraphicsProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg);
    if (msg->CheckId(UpdateTransform::Id))
    {
        this->UpdateTransform((msg.downcast<BaseGameFeature::UpdateTransform>())->GetMatrix(), false);
    }
    if (msg->CheckId(SetVisibleMsg::Id))
    {
        this->SetVisible((msg.upcast<SetVisibleMsg>())->GetVisible());
    }
    else if (msg->CheckId(GetGraphicsEntities::Id))
    {
        (msg.downcast<GetGraphicsEntities>())->SetEntities(this->graphicsEntities);
    }
    else
    {
        Property::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**    
    Called to update the graphics entity's transform.
*/
void
GraphicsProperty::UpdateTransform(const matrix44& m, bool setDirectly)
{
    // TODO: DO NOT SEARCH FOR A ATTACHED PHYSICS PROPERTY!
#if __USE_PHYSICS__    
   /* // if physics property exists, gather transforms from physics property,
    // otherwise use the transform in the UpdateTransform message
    PhysicsProperty* physProperty = (PhysicsProperty*) GetEntity()->FindProperty(PhysicsProperty::RTTI).get();
    if (!setDirectly && (0 != physProperty))
    {
        // gather transform from physics entity
        if (physProperty->IsEnabled())
        {
            Physics::PhysicsEntity* physEntity = physProperty->GetPhysicsEntity();
            if (physEntity)
            {
                Util::PhysicsGfxUtil::UpdateGraphicsTransforms(physEntity, this->graphicsEntities);
            }
        }
        
    }
    else*/
#endif
    {
        // set transformation directly
        uint i;
        for (i = 0; i < this->graphicsEntities.Size(); i++)
        {
            this->graphicsEntities[i]->SetTransform(m);
        }
    }
}

//------------------------------------------------------------------------------
/**    
    Shows or hides all attached graphics entities. 
*/
void 
GraphicsProperty::SetVisible(bool visible)
{
    if (visible)
    {
        // show
        IndexT i;
        for (i = 0; i < this->graphicsEntities.Size(); i++)
        {
            this->graphicsEntities[i]->SetVisible(true);
        }
    }
    else
    {
        // hide
        IndexT i;
        for (i = 0; i < this->graphicsEntities.Size(); i++)
        {
            this->graphicsEntities[i]->SetVisible(false);
        }
    }
}

}; // namespace GraphicsFeature