//------------------------------------------------------------------------------
//  util/charenhancementutil.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsutil/charenhancementutil.h"
#include "graphics/actorentity.h"
#include "graphics/stage.h"
#include "graphics/graphicsserver.h"
#include "msg/setanimation.h"
#include "msg/fadeanimation.h"
#include "managers/categorymanager.h"
#include "basegametiming/systemtimesource.h"
#include "graphicsfeatureunit.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::CharEnhancementUtil, 'CHEU', Core::RefCounted);

using namespace Math;
using namespace Nebula2;

//------------------------------------------------------------------------------
/**
*/
CharEnhancementUtil::CharEnhancementUtil()
{
    // empty
}

//------------------------------------------------------------------------------
/** Destructor
*/
CharEnhancementUtil::~CharEnhancementUtil()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This adds an attachment by resource name or directly with the
    provided graphics entity. Will create a new graphics
    entity, and attach it to the graphics level. This method
    will not remove existing attachments from the joint, if this is
    needed, call one of the Remove*Attachment() methods before. The method
    will however check, if an identical attachment already exists, and if this
    is the case, will do nothing. Also, if the joint is not found, the
    method will fail silently.

    @param  jointName       name of joint to add attachment to
    @param  resName         resource name, graphics entity will be created on the fly
    @param  graphicsEntity  optional existing graphics entity
    @param  offset          offset matrix
    @param  animMapping     optional anim mapping column of anim.xml for char attachments
    @param  attachEntity    true if the graphics entity should be attached to the graphics level
    @param  charEntity      create the new garphics entity as char entity
*/
void
CharEnhancementUtil::AddAttachment(const Util::String& jointName, 
                                      const Util::String& resName,
                                      Graphics::ModelEntity* graphicsEntity,
                                      const matrix44& offset,
                                      const Util::String& animMapping,
                                      bool attachEntity,
                                      bool charEntity)
{
    n_assert(jointName.IsValid());
    n_assert(resName.IsValid() || (0 != graphicsEntity));

    // if the attachment already exists, do nothing
    if (this->HasAttachments(jointName, resName, graphicsEntity))
    {
        return;
    }

    // setup a new attachment structure
    Attachment newAttachment;
    newAttachment.jointIndex = this->charGraphicsEntity->GetJointIndexByName(jointName);
    if (-1 != newAttachment.jointIndex)
    {
        // setup attachment info structure
        newAttachment.offsetMatrix = offset;
        if (0 == graphicsEntity)
        {
            if (charEntity)
            {
                n_assert(animMapping.IsValid());

                Ptr<Graphics::ActorEntity> charEntity = Graphics::ActorEntity::Create();
                charEntity->SetAnimationMapping(animMapping);
                charEntity->SetResourceId(resName);                
                newAttachment.graphicsEntity = charEntity;
            }
            else
            {
                newAttachment.graphicsEntity = Graphics::ModelEntity::Create();
                newAttachment.graphicsEntity->SetResourceId(resName);
            }
        }
        else
        {
            newAttachment.graphicsEntity = graphicsEntity;
        }
        newAttachment.graphicsEntity->SetVisible(false);
        newAttachment.wasAttachedToLevel = attachEntity;
        newAttachment.firstFrame = true;
        newAttachment.isCharEntity = charEntity;
        this->attachments.Append(newAttachment);
        
        // append the graphics entity to the level
        if (attachEntity)
        {
            GraphicsFeatureUnit::Instance()->GetDefaultStage()->AttachEntity(newAttachment.graphicsEntity.upcast<Graphics::GraphicsEntity>());
        }
    }

}

//------------------------------------------------------------------------------
/**
    This adds an attachment by resource name or directly with the
    provided graphics entity. Will create a new graphics
    entity, and attach it to the graphics level. This method
    will not remove existing attachments from the joint, if this is
    needed, call one of the Remove*Attachment() methods before. The method
    will however check, if an identical attachment already exists, and if this
    is the case, will do nothing. Also, if the joint is not found, the
    method will fail silently.

    @param  jointName       name of joint to add attachment to
    @param  resName         resource name, graphics entity will be created on the fly
    @param  graphicsEntity  optional existing graphics entity
    @param  offset          offset matrix
    @param  attachEntity    true if the graphics entity should be attached to the graphics level
    @param  charEntity      create the new garphics entity as char entity
*/
void
CharEnhancementUtil::AddAttachment(const Util::String& jointName, 
                                      const Util::String& resName,
                                      Graphics::ModelEntity* graphicsEntity,
                                      const matrix44& offset,
                                      bool attachEntity,
                                      bool charEntity)
{
    // set the parent char entity mapping as default
    const Util::String& animMapping = this->GetCharacterGraphicsEntity()->GetAnimationMapping();

    // attach the gfx entity
    this->AddAttachment(jointName, resName, graphicsEntity, offset, animMapping, attachEntity, charEntity);
}

//------------------------------------------------------------------------------
/**
    Remove attachments from the entity depending on the provided parameters.

    @param  jointName           optional joint name, consider all joints if empty
    @param  resName             optional resource name, check graphics entity pointer if empty
    @param  graphicsEntity      optional graphics entity pointer, consider all attachments if empty
    @param  graphicsEntityType  optional graphics entity type filter, consider all if InvalidType
*/
void
CharEnhancementUtil::RemoveAttachments(const Util::String& jointName,
                                          const Util::String& resName,
                                          Graphics::ModelEntity* graphicsEntity,
                                          Graphics::ModelEntity::Type graphicsEntityType)
{
    const Ptr<Graphics::Stage>& graphicsLevel = GraphicsFeatureUnit::Instance()->GetDefaultStage();
    int jointIndex = -1;
    if (jointName.IsValid())
    {
        // only check one joint
        jointIndex = this->charGraphicsEntity->GetJointIndexByName(jointName);
    }

    // for each attachment...
    IndexT i;
    for (i = 0; i < this->attachments.Size();)
    {
        bool remAttachment = true;
        const Attachment& curAttachment = this->attachments[i];
        if ((jointIndex != -1) && (jointIndex != curAttachment.jointIndex))
        {
            remAttachment = false;
        }
        else if (resName.IsValid() && (resName != curAttachment.graphicsEntity->GetResourceId().Value()))
        {
            remAttachment = false;
        }
        else if ((0 != graphicsEntity) && (graphicsEntity != curAttachment.graphicsEntity.get()))
        {
            remAttachment = false;
        }
        else if ((graphicsEntityType != Graphics::ModelEntity::InvalidType) && (graphicsEntityType != curAttachment.graphicsEntity->GetType()))
        {
            remAttachment = false;
        }
        if (remAttachment)
        {
            if (this-> attachments[i].wasAttachedToLevel)
            {
                // TODO:
                //// check for particle attachments
                //Util::ParticleShapeNodeHelper helper;
                //helper.SetEntity(this->attachments[i].graphicsEntity);
                //helper.FindEmitterNodes();
                //if(helper.HasEmittingParticleEmitters())
                //{
                //    helper.StopAllEmitters();           
                //    graphicsLevel->RemoveEntityDelayed(this->attachments[i].graphicsEntity, helper.GetMaxParticleLifetime());
                //}
                //else
                {
                    graphicsLevel->RemoveEntity(this->attachments[i].graphicsEntity.upcast<Graphics::GraphicsEntity>());
                }
            }
            this->attachments.EraseIndex(i);
        }
        else
        {
            i++;
        }
    }
}

//------------------------------------------------------------------------------
/**
    Set base and/or overlay animation to attachments. 
*/
void
CharEnhancementUtil::SetAnimation(GraphicsFeature::SetAnimation* msg)
{
    n_assert(0 != msg);

    // for each attachment...
    int numAttachments = this->attachments.Size();
    int idxAttachment;
    for (idxAttachment = 0; idxAttachment < numAttachments; idxAttachment++)
    {
        Attachment& attachment = this->attachments[idxAttachment];
        if (attachment.isCharEntity)
        {
            n_assert(attachment.graphicsEntity->IsA(Graphics::ActorEntity::RTTI));
            Graphics::ActorEntity* gfxEntity = (Graphics::ActorEntity*)attachment.graphicsEntity.get();

            if (msg->GetBaseAnimation().IsValid())
            {
                if (msg->GetBaseAnimation() != gfxEntity->GetBaseAnimation())
                {
                    gfxEntity->SetBaseAnimation(msg->GetBaseAnimation(), msg->GetFadeInTime(), msg->GetBaseAnimTimeOffset(), true, true, msg->GetRandomValue());
                }
            }
            if (msg->GetOverlayAnimStop())
            {
                gfxEntity->StopOverlayAnimation(msg->GetFadeInTime());
            }
            if (msg->GetOverlayAnimation().IsValid())
            {
                gfxEntity->SetOverlayAnimation(msg->GetOverlayAnimation(), msg->GetFadeInTime(), msg->GetOverlayAnimDurationOverride(), true, msg->GetRandomValue());
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    Fade an animation(-mix)
*/
void
CharEnhancementUtil::FadeAnimation(GraphicsFeature::FadeAnimation* msg)
{
    n_assert(0 != msg);

    // for each attachment...
    int numAttachments = this->attachments.Size();
    int idxAttachment;
    for (idxAttachment = 0; idxAttachment < numAttachments; idxAttachment++)
    {
        Attachment& attachment = this->attachments[idxAttachment];
        if (attachment.isCharEntity)
        {
            n_assert(attachment.graphicsEntity->IsA(Graphics::ActorEntity::RTTI));
            Graphics::ActorEntity* gfxEntity = (Graphics::ActorEntity*)attachment.graphicsEntity.get();

            if(msg->GetAnimationName().IsValid())
            {
                gfxEntity->FadeAnimation(msg->GetAnimationName(), msg->GetAnimationWeight(), msg->GetFadeInTime(), msg->GetFadeOutTime(), msg->GetRandomValue(), msg->GetRunTime(), msg->GetTimeFactor(), msg->GetSampleTime(), msg->IsRestartAnimSet(), msg->IsFadeOutRunningAnimsSet());
            }
            if(msg->GetAnimationMixNames().Size() > 0 && msg->GetAnimationMixWeights().Size() == 0)
            {
                gfxEntity->FadeAnimationMix(msg->GetAnimationMixNames(), msg->GetAnimationWeight(), msg->GetFadeInTime(), msg->GetFadeOutTime(), msg->GetRandomValue(), msg->GetRunTime(), msg->GetTimeFactor(), msg->GetSampleTime(), msg->IsRestartAnimSet(), msg->IsFadeOutRunningAnimsSet());
            }
            if(msg->GetAnimationMixNames().Size() > 0 && msg->GetAnimationMixNames().Size() == msg->GetAnimationMixWeights().Size())
            {
                gfxEntity->FadeAnimationMix(msg->GetAnimationMixNames(), msg->GetAnimationMixWeights(), msg->GetFadeInTime(), msg->GetFadeOutTime(), msg->GetRandomValue(), msg->GetRunTime(), msg->GetTimeFactor(), msg->GetSampleTime(), msg->IsRestartAnimSet(), msg->IsFadeOutRunningAnimsSet());
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    Return an array of matching attachment indices.
*/
Util::Array<IndexT>
CharEnhancementUtil::GetMatchingAttachmentIndices(const Util::String& jointName, 
                                                     const Util::String& resName, 
                                                     Graphics::ModelEntity* graphicsEntity, 
                                                     Graphics::ModelEntity::Type graphicsEntityType) const
{
    Util::Array<IndexT> result;
    int jointIndex = -1;
    if (jointName.IsValid())
    {
        // only check one joint
        jointIndex = this->charGraphicsEntity->GetJointIndexByName(jointName);
    }

    // for each attachment...
    IndexT i;
    for (i = 0; i < this->attachments.Size(); i++)
    {
        const Attachment& curAttachment = this->attachments[i];
        if ((jointIndex != -1) && (jointIndex != curAttachment.jointIndex))
        {
            // not a match...
            continue;
        }
        else if (resName.IsValid() && (resName != curAttachment.graphicsEntity->GetResourceId().Value()))
        {
            // not a match...
            continue;
        }
        else if ((0 != graphicsEntity) && (graphicsEntity != curAttachment.graphicsEntity.get()))
        {
            // not a match...
            continue;
        }
        else if ((graphicsEntityType != Graphics::ModelEntity::InvalidType) && (graphicsEntityType != curAttachment.graphicsEntity->GetType()))
        {
            // not a match...
            continue;
        }
        // a match!
        result.Append(i);
    }
    return result;
}

//------------------------------------------------------------------------------
/**
    This checks if at least one joint attachment exists matching the 
    given filter arguments. Same rules apply as RemoveAttachments().

    @param  jointName           optional joint name, consider all joints if empty
    @param  resName             optional resource name, check graphics entity pointer if empty
    @param  graphicsEntity      optional graphics entity pointer, consider all attachments if empty
    @param  graphicsEntityType  optional graphics entity type filter, consider all if InvalidType
*/
bool
CharEnhancementUtil::HasAttachments(const Util::String& jointName, 
                                       const Util::String& resName, 
                                       Graphics::ModelEntity* graphicsEntity, 
                                       Graphics::ModelEntity::Type graphicsEntityType) const
{
    Util::Array<IndexT> matchingIndices = this->GetMatchingAttachmentIndices(jointName, resName, graphicsEntity, graphicsEntityType);
    return matchingIndices.Size() > 0;
}

//------------------------------------------------------------------------------
/**
    This hides all matching attachments by setting their graphics entities
    to hidden.
*/
void
CharEnhancementUtil::HideAttachments(const Util::String& jointName, 
                                        const Util::String& resName, 
                                        Graphics::ModelEntity* graphicsEntity, 
                                        Graphics::ModelEntity::Type graphicsEntityType) const
{
    Util::Array<IndexT> matchingIndices = this->GetMatchingAttachmentIndices(jointName, resName, graphicsEntity, graphicsEntityType);
    IndexT i;
    for (i = 0; i < matchingIndices.Size(); i++)
    {
        this->attachments[matchingIndices[i]].graphicsEntity->SetVisible(false);
		this->attachments[matchingIndices[i]].firstFrame = false;
    }
}

//------------------------------------------------------------------------------
/**
    This hides all matching attachments by setting their graphics entities
    to hidden.
*/
void
CharEnhancementUtil::ShowAttachments(const Util::String& jointName, 
                                        const Util::String& resName, 
                                        Graphics::ModelEntity* graphicsEntity, 
                                        Graphics::ModelEntity::Type graphicsEntityType) const
{
    Util::Array<IndexT> matchingIndices = this->GetMatchingAttachmentIndices(jointName, resName, graphicsEntity, graphicsEntityType);
    IndexT i;
    for (i = 0; i < matchingIndices.Size(); i++)
    {
        this->attachments[matchingIndices[i]].graphicsEntity->SetVisible(true);
    }
}

//------------------------------------------------------------------------------
/**
    Updates the transformations of all attachments.
*/
void
CharEnhancementUtil::UpdateAttachments()
{
    if (this->attachments.Size() > 0)
    {
        // update Nebula character skeleton: enforce evaluation -> update anim clips
        this->charGraphicsEntity->EvaluateSkeleton(true);

        // get my entity's transformation
        const matrix44& worldMatrix = this->charGraphicsEntity->GetTransform();

        // a correctional matrix which rotates the joint matrix by 180
        // degree around global Y to account for the fact the Nebula2
        // characters look along +Z, not -Z
        matrix44 rot180 = matrix44::rotationy(n_deg2rad(180.0f));

        // for each attachment...
        int numAttachments = this->attachments.Size();
        int i;
        for (i = 0; i < numAttachments; i++)
        {
            Attachment& curAttachment = this->attachments[i];
            
            // get attachment joint matrix...
            matrix44 jointMatrix = curAttachment.offsetMatrix;
            jointMatrix = matrix44::multiply(jointMatrix, this->charGraphicsEntity->GetLocalJointMatrix(curAttachment.jointIndex));

            // rotate the matrix by 180 degree (since Nebula2 characters look into -z)!!                
            jointMatrix = matrix44::multiply(jointMatrix, rot180);
            jointMatrix = matrix44::multiply(jointMatrix, worldMatrix);

            // update the graphics entity's transform
            curAttachment.graphicsEntity->SetTransform(jointMatrix);
            
            // make the attachment visible if this hasn't happened yet
            // (the attachment will be set to invisible when attached
            // to prevent unwanted frame plopping)
            if (curAttachment.firstFrame)
            {
                curAttachment.graphicsEntity->SetVisible(true);
                curAttachment.firstFrame = false;
            }
        }
    }
}

//------------------------------------------------------------------------------
/** 
    Return an array with the gfx-entities of the attachments. NOTE:
    this is a slow method!
*/
Util::Array<Ptr<Graphics::ModelEntity> >
CharEnhancementUtil::GetAttachmentGraphicsEntities() const
{
    Util::Array<Ptr<Graphics::ModelEntity> > entities;
    IndexT i;
    for (i = 0; i < this->attachments.Size(); ++i)
    {
        n_assert(this->attachments[i].graphicsEntity.isvalid());
        entities.Append(this->attachments[i].graphicsEntity);
    }
    return entities;
}

//------------------------------------------------------------------------------
/**
    Checks if the given Skin exists for Character3. 
*/
bool
CharEnhancementUtil::HasSkin(const Util::String& skinName) const
{
    n_assert(skinName.IsValid());
    if (this->charGraphicsEntity->HasCharacter3Set())
    {
        Nebula2::nCharacter3Set* character3Set = this->charGraphicsEntity->GetCharacter3Set();
        n_assert(0 != character3Set);
        return character3Set->HasSkin(skinName);
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Makes the given skin visible if our graphics entity has a Character3.
*/
bool
CharEnhancementUtil::AddSkin(const Util::String& skinName )
{
    n_assert(skinName.IsValid());
    if (this->charGraphicsEntity->HasCharacter3Set())
    {
        nCharacter3Set* character3Set = this->charGraphicsEntity->GetCharacter3Set();
        n_assert(character3Set);
        if (this->HasSkin(skinName))
        {
            character3Set->SetSkinVisible(skinName, true);
            return true;    
        }
        else
        {
            n_printf("*** RESOURCE ERROR: missing skin list '%s' on character '%s'\n",
                skinName.AsCharPtr(), this->charGraphicsEntity->GetResourceId().Value().AsCharPtr());
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Makes the given skin invisible if our graphics entity has a Character3.
*/
bool
CharEnhancementUtil::RemSkin(const Util::String& skinName)
{
    n_assert(skinName.IsValid());
    if (this->charGraphicsEntity->HasCharacter3Set())
    {
        nCharacter3Set* character3Set = (nCharacter3Set*) this->charGraphicsEntity->GetCharacter3Set();
        n_assert(character3Set);
        if (this->HasSkin(skinName))
        {
            character3Set->SetSkinVisible(skinName, false);
            return true;
        }
        else
        {
            n_printf("*** RESOURCE ERROR: missing skin list '%s' on character '%s'\n",
                skinName.AsCharPtr(), this->charGraphicsEntity->GetResourceId().Value().AsCharPtr());
        }
    }
    return false;
}

} // namespace GraphicsFeature