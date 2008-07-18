//------------------------------------------------------------------------------
//  properties/actorgraphicsproperty.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "properties/actorgraphicsproperty.h"

#include "msg/addattachment.h"
#include "msg/remattachment.h"
#include "msg/getattachmententities.h"
#include "msg/updateattachments.h"
#include "msg/setanimation.h"
#include "msg/fadeanimation.h"
#include "msg/getanimationinfo.h"
#include "msg/hasattachment.h"
#include "msg/hideattachment.h"
#include "msg/showattachment.h"
#include "msg/addskin.h"
#include "msg/remskin.h"
#include "msg/getjointmatrix.h"
#include "msg/gethotspottime.h"
#include "msg/updatetransform.h"
#include "msg/movedirection.h"
#include "msg/movestop.h"

#include "core/factory.h"
#include "game/entity.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "resources/resource.h"
#if __USE_PHYSICS__
#include "properties/actorphysicsproperty.h"
#endif
#include "basegametiming/timemanager.h"
#include "managers/categorymanager.h"
#include "nebula2/nebula2wrapper.h"
#include "graphicsfeatureunit.h"

namespace Attr
{
    DefineString(AnimSet, 'ANST', Attr::ReadWrite);
    DefineString(CharacterSet, 'CHST', Attr::ReadWrite);
}

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::ActorGraphicsProperty, 'A', GraphicsFeature::GraphicsProperty);

using namespace BaseGameFeature;
using namespace Util;
using namespace Math;
using namespace Graphics;
using namespace Nebula2;

//------------------------------------------------------------------------------
/**
*/
ActorGraphicsProperty::ActorGraphicsProperty()
{
}

//------------------------------------------------------------------------------
/**
*/
ActorGraphicsProperty::~ActorGraphicsProperty()
{
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::OnActivate()
{  
    Property::OnActivate();
    this->SetupGraphicsEntities();
    this->charEnhancementUtil = CharEnhancementUtil::Create();
    this->charEnhancementUtil->SetCharacterGraphicsEntity(this->GetGraphicsEntity());

	// create the hotspot notify message
    this->gfxHotspotTriggeredMsg = AnimationHotspotTriggered::Create();

    // set idle animation    
    Graphics::ActorEntity* Entity = this->GetGraphicsEntity();
    Entity->SetBaseAnimation("idle_01", 0.0f, 0.0f, true, true, n_rand(0, 1.0));    
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::OnDeactivate()
{
    // remove all attachments
    this->charEnhancementUtil->RemoveAttachments("", "", 0);
    this->charEnhancementUtil = 0;

	// remove hotspot notify message
    this->gfxHotspotTriggeredMsg = 0;

    // skip this one -> GraphicsProperty::OnDeactivate(); -> we do it self here (maybe delayed job)
    
    // current level
    Graphics::Stage* stage = GraphicsFeatureUnit::Instance()->GetDefaultStage();

    // release graphics entities -> but delayed
    int i;
    int num = this->graphicsEntities.Size();
    for (i = 0;  i < num; i++)
    {
        Graphics::ModelEntity* Entity = this->graphicsEntities[i];
        if (Entity->GetCell())
        {
            //Util::ParticleShapeNodeHelper helper;
            //helper.SetEntity(this->graphicsEntities[i]);
            //helper.FindEmitterNodes();
            //if(helper.HasEmittingParticleEmitters())
            //{
            //    helper.StopAllEmitters();           
            //    Level->RemoveEntityDelayed(this->graphicsEntities[i], helper.GetMaxParticleLifetime());
            //}
            //else
            {
                stage->RemoveEntity(this->graphicsEntities[i].upcast<Graphics::GraphicsEntity>());
            }
        }
        this->graphicsEntities[i] = 0;
    }
    this->graphicsEntities.Clear();

    // now call parents parent class
    Property::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::SetupCallbacks()
{
    this->entity->RegisterPropertyCallback(this, Render);
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::SetupAcceptedMessages()
{
    this->RegisterMessage(AddAttachment::Id);
    this->RegisterMessage(RemAttachment::Id);
    this->RegisterMessage(GetAttachmentEntities::Id);
    this->RegisterMessage(UpdateAttachments::Id);
    this->RegisterMessage(SetAnimation::Id);
    this->RegisterMessage(AddSkin::Id);
    this->RegisterMessage(RemSkin::Id);
    this->RegisterMessage(UpdateTransform::Id);
    this->RegisterMessage(GetJointMatrix::Id);
    this->RegisterMessage(GetActiveAnimation::Id);
    this->RegisterMessage(GetAnimationInfo::Id);
    this->RegisterMessage(FadeAnimation::Id);
    this->RegisterMessage(HasAttachment::Id);
    this->RegisterMessage(HideAttachment::Id);
    this->RegisterMessage(ShowAttachment::Id);
    this->RegisterMessage(GetHotspotTime::Id);
    this->RegisterMessage(SetFadeAnimationMix::Id);
    this->RegisterMessage(MoveDirection::Id);
    this->RegisterMessage(MoveStop::Id);
    GraphicsProperty::SetupAcceptedMessages();
}


//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg);
    if (msg->CheckId(AddAttachment::Id))
    {
        const Ptr<AddAttachment>& AddAtt = msg.downcast<AddAttachment>();
        const Util::String& jointName    = AddAtt->GetJointName();
        const Util::String& resName      = AddAtt->GetResourceName();
        const matrix44& offset           = AddAtt->GetOffsetMatrix();
        Graphics::ModelEntity* Entity    = AddAtt->GetGraphicsEntity();
        bool remCurAttachments           = AddAtt->GetRemCurAttachment();
        bool attachEntityToLevel         = AddAtt->GetAttachEntityToLevel();
        bool isCharGraphics              = AddAtt->IsCharGraphics();
        const Util::String& animMapping  = AddAtt->GetAnimationMapping();

        if (remCurAttachments)
        {
            // remove all attachments on that joint
            this->charEnhancementUtil->RemoveAttachments(jointName, "", 0);
        }
        // add new attachments
        this->charEnhancementUtil->AddAttachment(jointName, resName, Entity, offset, animMapping, attachEntityToLevel, isCharGraphics);
    }
    else if (msg->CheckId(RemAttachment::Id))
    {
        const Ptr<RemAttachment>& RemAtt = msg.downcast<RemAttachment>();
        const Util::String& jointName          = RemAtt->GetJointName();
        const Util::String& resName            = RemAtt->GetResourceName();
        Graphics::ModelEntity::Type entityType = RemAtt->GetGraphicsEntityType();
        Graphics::ModelEntity* Entity       = RemAtt->GetGraphicsEntity();
        this->charEnhancementUtil->RemoveAttachments(jointName, resName, Entity, entityType);
    }
    else if (msg->CheckId(HasAttachment::Id))
    {
        const Ptr<HasAttachment>& HasAtt = msg.downcast<HasAttachment>();
        const Util::String& jointName = HasAtt->GetJointName();
        const Util::String& resName   = HasAtt->GetResourceName();
        Graphics::ModelEntity::Type entityType = HasAtt->GetGraphicsEntityType();
        HasAtt->SetResult(this->charEnhancementUtil->HasAttachments(jointName, resName, 0, entityType));
    }
    else if (msg->CheckId(HideAttachment::Id))
    {
        const Ptr<HideAttachment>& HideAtt = msg.downcast<HideAttachment>();
        const Util::String& jointName = HideAtt->GetJointName();
        const Util::String& resName   = HideAtt->GetResourceName();
        Graphics::ModelEntity::Type entityType = HideAtt->GetGraphicsEntityType();
        this->charEnhancementUtil->HideAttachments(jointName, resName, 0, entityType);
    }
    else if (msg->CheckId(ShowAttachment::Id))
    {
        const Ptr<ShowAttachment>& ShowAtt = msg.downcast<ShowAttachment>();
        const Util::String& jointName = ShowAtt->GetJointName();
        const Util::String& resName   = ShowAtt->GetResourceName();
        Graphics::ModelEntity::Type entityType = ShowAtt->GetGraphicsEntityType();
        this->charEnhancementUtil->ShowAttachments(jointName, resName, 0, entityType);
    }
    else if (msg->CheckId(GetAttachmentEntities::Id))
    {
        const Ptr<GetAttachmentEntities> GetEntitiesMessage = msg.downcast<GetAttachmentEntities>();
        GetEntitiesMessage->SetEntities(this->charEnhancementUtil->GetAttachmentGraphicsEntities());
    }
    else if (msg->CheckId(UpdateAttachments::Id))
    {
        this->charEnhancementUtil->UpdateAttachments();
    }
    else if (msg->CheckId(GraphicsFeature::SetAnimation::Id))
    {
        this->SetAnimation(msg.downcast<GraphicsFeature::SetAnimation>());
    }
    else if (msg->CheckId(SetFadeAnimationMix::Id))
    {
        this->SetFadeAnimation(msg.downcast<SetFadeAnimationMix>());
    } 
    else if (msg->CheckId(GraphicsFeature::FadeAnimation::Id))
    {
        this->FadeAnimation(msg.downcast<GraphicsFeature::FadeAnimation>());
    }
    else if (msg->CheckId(GraphicsFeature::AddSkin::Id))
    {
        const Ptr<GraphicsFeature::AddSkin>& AddSkin = msg.downcast<GraphicsFeature::AddSkin>();

        Util::String name = AddSkin->GetSkinName();

        if(!name.IsEmpty() && !this->charEnhancementUtil->AddSkin(name))
        {
            n_printf("ActorGraphicsProperty::AddSkin: invalid skin '%s' on entity '%s'\n(character set is %s)", 
                AddSkin->GetSkinName().AsCharPtr(), this->entity->GetString(Attr::Id).AsCharPtr(), this->entity->GetString(Attr::Graphics).AsCharPtr());
        }
    }
    else if (msg->CheckId(GraphicsFeature::RemSkin::Id))
    {
        const Ptr<GraphicsFeature::RemSkin>& RemSkin = msg.downcast<GraphicsFeature::RemSkin>();

        Util::String name = RemSkin->GetSkinName();
        if(!name.IsEmpty() && !this->charEnhancementUtil->RemSkin(name))
        {
            n_printf("ActorGraphicsProperty::RemSkin: invalid skin '%s' on entity '%s'\n(character set is %s)", 
                RemSkin->GetSkinName().AsCharPtr(), this->entity->GetString(Attr::Id).AsCharPtr(), this->entity->GetString(Attr::Graphics).AsCharPtr());
        }
    }
    else if (msg->CheckId(UpdateTransform::Id))
    {
        const Ptr<GraphicsFeature::UpdateTransform>& updTransform = msg.downcast<GraphicsFeature::UpdateTransform>();
        this->graphicsEntities[0]->SetTransform(updTransform->GetMatrix());
    }
    else if (msg->CheckId(GetJointMatrix::Id))
    {
        const Ptr<GetJointMatrix>& getJointMatrixMsg = msg.downcast<GetJointMatrix>();
        const Util::String& jointName = getJointMatrixMsg->GetJointName();
        
        const Ptr<Graphics::ActorEntity>& Entity = this->GetGraphicsEntity();
        IndexT jointIndex = Entity->GetJointIndexByName(jointName);
        if (InvalidIndex != jointIndex)
        {
			if(getJointMatrixMsg->IsLocalJointMatrixOnly())
			{
				getJointMatrixMsg->SetMatrix(Entity->GetLocalJointMatrix(jointIndex));			
			}
			else
			{
				getJointMatrixMsg->SetMatrix(Entity->GetJointMatrixByIndex(jointIndex));
			}
        }
    }
    else if (msg->CheckId(GetActiveAnimation::Id))
    {
        const Ptr<GetActiveAnimation>& getAnimMsg = msg.downcast<GetActiveAnimation>();
        this->HandleGetActiveAnimation(getAnimMsg);
    }
    else if (msg->CheckId(GetAnimationInfo::Id))
    {
        const Ptr<GetAnimationInfo>& animInfoMsg = msg.downcast<GetAnimationInfo>();
        
        Util::String animName = animInfoMsg->GetAnimationName();
        n_assert(0 != animName.Length());
        
        Graphics::ActorEntity* Entity = this->GetGraphicsEntity();
        const Util::String& clipName = Nebula2::AnimTable::Instance()->GetMappedAnimName(Entity->GetAnimationMapping(), animName);
        IndexT clipIndex = Entity->GetCharacterNode()->GetClipIndexByName(clipName);

        animInfoMsg->SetDuration(Entity->GetAnimClipScheduler()->GetClipDurationAt(clipIndex));
        animInfoMsg->SetFadeInTime(Entity->GetAnimClipScheduler()->GetClipFadeInTimeAt(clipIndex));
        animInfoMsg->SetFadeOutTime(Entity->GetAnimClipScheduler()->GetClipFadeOutTimeAt(clipIndex));
        animInfoMsg->SetRunTime(Entity->GetAnimClipScheduler()->GetClipRunTimeAt(clipIndex));
        animInfoMsg->SetSampleTime(Entity->GetAnimClipScheduler()->GetClipSampleTimeAt(clipIndex));
        animInfoMsg->SetTimeFactor(Entity->GetAnimClipScheduler()->GetClipTimeFactorAt(clipIndex));
        animInfoMsg->SetWeight(Entity->GetAnimClipScheduler()->GetClipWeightAt(clipIndex));
    }
    else if (msg->CheckId(GetHotspotTime::Id))
    {
        /*const Ptr<GetHotspotTime>& hotspotMsg = msg.downcast<GetHotspotTime>();

        const Util::String& animName = hotspotMsg->GetAnimationName();
        const Util::String& hotspotName = hotspotMsg->GetHotspotName();
        n_assert(0 != animName.Length());
        
        ActorEntity* Entity = this->GetGraphicsEntity();
        AnimTable::AnimInfo animInfo = AnimTable::Instance()->GetMappedAnimName(Entity->GetAnimationMapping(), animName);
        
        float hotspotTime = animInfo.GetHotspotFrame(hotspotName);
        
        float timesourceFactor = 1.0f;
        if(Entity->GetTimeSource())
        {
            timesourceFactor = Entity->GetTimeSource()->GetFactor();
        }
        float timeFactor = Entity->GetTimeFactor() * timesourceFactor;
        hotspotTime = hotspotTime * timeFactor * 0.04f;

        hotspotMsg->SetHotspotTime(hotspotTime);*/
    }
    // TODO: move to extra property, just for testing
    else if (msg->CheckId(MoveDirection::Id))
    {
        // set idle animation    
        Graphics::ActorEntity* Entity = this->GetGraphicsEntity();
        Entity->SetBaseAnimation("laufen_01", 0.2f, 0.0f, true, true, 0.2f);  
    }
    else if (msg->CheckId(MoveStop::Id))
    {
        // set idle animation    
        Graphics::ActorEntity* Entity = this->GetGraphicsEntity();
        Entity->SetBaseAnimation("idle_01", 0.2f, 0.0f, true, true, 0.2f);  
    }
    else
    {
        GraphicsProperty::HandleMessage(msg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::SetupDefaultAttributes()
{
    GraphicsProperty::SetupDefaultAttributes();
    this->GetEntity()->AddString(Attr::AnimSet);
    this->GetEntity()->AddString(Attr::CharacterSet);
}

//------------------------------------------------------------------------------
/**
    This sets up a single character graphics entity. This method is called
    by OnActivate() of the parent class.
*/
void
ActorGraphicsProperty::SetupGraphicsEntities()
{
    n_assert(this->entity->HasAttr(Attr::Transform));
    
    // create and setup graphics property
	Ptr<Graphics::ActorEntity> ge = Graphics::ActorEntity::Create();
    Util::String fullResourceName("mdl:");
    fullResourceName.Append(this->GetGraphicsResource());
    fullResourceName.Append(".n2");
    ge->SetResourceId(fullResourceName);
    String skinListPath;
    skinListPath.Format("mdl:%s/skinlists/%s.xml", this->GetGraphicsResource().AsCharPtr(), this->entity->GetString(Attr::CharacterSet).AsCharPtr());
    ge->SetSkinList(skinListPath);
    ge->SetTransform(this->entity->GetMatrix44(Attr::Transform));
    Util::String animMapping = this->entity->GetString(Attr::AnimSet);
    if (animMapping.IsValid())
    {
        ge->SetAnimationMapping(animMapping);
    }
    else
    {
        n_error("ActorGraphicsProperty::SetupGraphicsEntity(): entity '%s' has empty AnimSet attribute!", 
            this->entity->GetString(Attr::Id).AsCharPtr());
    }
    
    // attach graphics property to level
    Graphics::Stage* graphicsLevel = GraphicsFeatureUnit::Instance()->GetDefaultStage();
    n_assert(graphicsLevel);
    graphicsLevel->AttachEntity(ge.upcast<Graphics::GraphicsEntity>());

    // set in parent class' graphics entity array
    this->graphicsEntities.Append(ge.get());
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::OnStart()
{
    GraphicsProperty::OnStart();
    
    // set last hotspot update time
    this->lastHotspotUpdateTime = this->GetGraphicsEntity()->GetTime();
}

//------------------------------------------------------------------------------
/**
    Set base and/or overlay animation. 
*/
void
ActorGraphicsProperty::SetAnimation(GraphicsFeature::SetAnimation* msg)
{
    n_assert(0 != msg);
    Graphics::ActorEntity* Entity = this->GetGraphicsEntity();
    if (msg->GetBaseAnimation().IsValid())
    {
        if (msg->GetBaseAnimation() != Entity->GetBaseAnimation())
        {
            Entity->SetBaseAnimation(msg->GetBaseAnimation(), msg->GetFadeInTime(), msg->GetBaseAnimTimeOffset(), true, true, msg->GetRandomValue());
        }
    }
    if (msg->GetOverlayAnimStop())
    {
        Entity->StopOverlayAnimation(msg->GetFadeInTime());
    }
    if (msg->GetOverlayAnimation().IsValid())
    {
        Entity->SetOverlayAnimation(msg->GetOverlayAnimation(), msg->GetFadeInTime(), msg->GetOverlayAnimDurationOverride(), true, msg->GetRandomValue());
    }

    this->charEnhancementUtil->SetAnimation(msg);
}

//------------------------------------------------------------------------------
/**
    set an fade animationmix
*/
void
ActorGraphicsProperty::SetFadeAnimation(SetFadeAnimationMix* msg)
{
    n_assert(0 != msg);
    Graphics::ActorEntity* Entity = this->GetGraphicsEntity();

    Entity->SetFadeAnimationMix(msg->GetAnimationMixNames(), msg->GetAnimationMixWeights(), msg->GetFadeInTime(), 0, 0);
}

//------------------------------------------------------------------------------
/**
    Fade an animation(-mix)
*/
void
ActorGraphicsProperty::FadeAnimation(GraphicsFeature::FadeAnimation* msg)
{
    n_assert(0 != msg);
    Graphics::ActorEntity* Entity = this->GetGraphicsEntity();

    if(msg->GetAnimationName().IsValid())
    {
        Entity->FadeAnimation(msg->GetAnimationName(), msg->GetAnimationWeight(), msg->GetFadeInTime(), msg->GetFadeOutTime(), msg->GetRandomValue(), msg->GetRunTime(), msg->GetTimeFactor(), msg->GetSampleTime(), msg->IsRestartAnimSet(), msg->IsFadeOutRunningAnimsSet());
    }
    if(msg->GetAnimationMixNames().Size() > 0 && msg->GetAnimationMixWeights().Size() == 0)
    {
        Entity->FadeAnimationMix(msg->GetAnimationMixNames(), msg->GetAnimationWeight(), msg->GetFadeInTime(), msg->GetFadeOutTime(), msg->GetRandomValue(), msg->GetRunTime(), msg->GetTimeFactor(), msg->GetSampleTime(), msg->IsRestartAnimSet(), msg->IsFadeOutRunningAnimsSet());
    }
    if(msg->GetAnimationMixNames().Size() > 0 && msg->GetAnimationMixNames().Size() == msg->GetAnimationMixWeights().Size())
    {
        Entity->FadeAnimationMix(msg->GetAnimationMixNames(), msg->GetAnimationMixWeights(), msg->GetFadeInTime(), msg->GetFadeOutTime(), msg->GetRandomValue(), msg->GetRunTime(), msg->GetTimeFactor(), msg->GetSampleTime(), msg->IsRestartAnimSet(), msg->IsFadeOutRunningAnimsSet());
    }

    this->charEnhancementUtil->FadeAnimation(msg);
}

//------------------------------------------------------------------------------
/**
    Called before rendering happens. This processes pending messages.
*/
void
ActorGraphicsProperty::OnRender()
{
    if (this->GetGraphicsEntity()->IsVisible())
    {
        this->charEnhancementUtil->UpdateAttachments();
    }

	// Get hotspot data and send them as message
    Util::Array<nAnimation::Group::AnimHotspotInfo>& hotspots = this->gfxHotspotTriggeredMsg->Hotspots();  
    hotspots.Clear();
    
    Timing::Time curTime = this->GetGraphicsEntity()->GetTime();
    /*Graphics::ActorEntity* Entity = this->GetGraphicsEntity();
    if (0 < Entity->GetCharacterPointer()->GetHotspotsInRange(this->lastHotspotUpdateTime, curTime, hotspots))
    {
        this->GetEntity()->SendSync(this->gfxHotspotTriggeredMsg.upcast<Messaging::Message>());
    }*/
    this->lastHotspotUpdateTime = curTime;
}

//------------------------------------------------------------------------------
/**
*/
void
ActorGraphicsProperty::HandleGetActiveAnimation(GetActiveAnimation *getAnimMsg)
{
    Util::String baseAnim;
    Util::String overlayAnim;

    if (getAnimMsg->IsClipNameRequested())
    {
        baseAnim = this->GetGraphicsEntity()->GetBaseClip();
        overlayAnim = this->GetGraphicsEntity()->GetOverlayClip();
    }
    else
    {
        baseAnim = this->GetGraphicsEntity()->GetBaseAnimation();
        overlayAnim = this->GetGraphicsEntity()->GetOverlayAnimation();
    }

    if (getAnimMsg->IsBaseAnimRequested())
    {
        getAnimMsg->SetAnimName(baseAnim);
    }
    else
    {
        if (overlayAnim.IsValid())
        {
            getAnimMsg->SetAnimName(overlayAnim);
        }
        else
        {
            getAnimMsg->SetAnimName(baseAnim);
        }
    }
}

}; // namespace GraphicsFeature

