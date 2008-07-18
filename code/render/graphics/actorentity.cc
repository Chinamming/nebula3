//------------------------------------------------------------------------------
//  ActorEntity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/actorentity.h"
#include "models/nodes/characternode.h"
#include "models/nodes/characternodeinstance.h"
#include "util/stringatom.h"

namespace Graphics
{
ImplementClass(Graphics::ActorEntity, 'ACEN', Graphics::ModelEntity);

using namespace Math;
using namespace Timing;
using namespace Util;
using namespace Models;
using namespace Anim;
using namespace Char;

//------------------------------------------------------------------------------
/**
*/
ActorEntity::ActorEntity() :
    firstBaseAnim(true),
    nebCharacter(0),
    characterSet(0),
    clipScheduler(0),
    character3Mode(false),
    characterNode(0),
    timeFactor(1.0f),
    requestAnim(false)
{
    this->baseAnimation.animDuration = 0.0;
    this->baseAnimation.animFadeIn = 0.0;
    this->baseAnimation.animOffset = 0.0;
    this->baseAnimation.animStarted = 0.0;
    this->baseAnimation.startAnim = false;

    this->overlayAnimation.animDuration = 0.0;
    this->overlayAnimation.animFadeIn = 0.0;
    this->overlayAnimation.animOffset = 0.0;
    this->overlayAnimation.animStarted = 0.0;
    this->overlayAnimation.startAnim = false;
}

//------------------------------------------------------------------------------
/**
*/
ActorEntity::~ActorEntity()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ActorEntity::OnActivate()
{
    n_assert(!this->characterSet.isvalid());
    n_assert(!this->nebCharacter.isvalid());
    // do not show all our nodes on startup, skinlist will set the correct ones visible
    // ModelEnity sets the visibility in onactivate
    if (this->skinList.IsValid())
    {
        this->setModelNodesVisible = false;
    }

    ModelEntity::OnActivate();

    this->ValidateCharacter();
}

//------------------------------------------------------------------------------
/**
*/
void
ActorEntity::OnDeactivate()
{
    if (this->nebCharacter)
    {
        this->nebCharacter = 0;
    }
    this->characterSet = 0;
    ModelEntity::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
ActorEntity::ValidateCharacter()
{
    if (this->GetModelInstance().isvalid() && this->GetModelInstance()->GetModel()->GetState() == Resources::Resource::Loaded)
    {
        if (!this->nebCharacter.isvalid())
        {
            const Ptr<Models::ModelInstance>& modelInstance = this->GetModelInstance();
            const Ptr<Models::ModelNodeInstance>& nodeInst = modelInstance->LookupNodeInstanceByRTTI(Models::CharacterNodeInstance::RTTI);
            n_assert(nodeInst->IsA(Models::CharacterNodeInstance::RTTI));
            const Ptr<Models::CharacterNodeInstance>& charNodeInst = nodeInst.downcast<Models::CharacterNodeInstance>();        
            this->characterNode = charNodeInst->GetModelNode().downcast<CharacterNode>();
            this->nebCharacter = charNodeInst->GetCharacter();
        }

        if (!this->clipScheduler.isvalid() 
            && this->nebCharacter.isvalid() 
            && this->nebCharacter->GetClipScheduler().isvalid())
        {
            // store clip scheduler pointer
            this->clipScheduler = this->nebCharacter->GetClipScheduler();    
            this->SetupCharacter3Set();  
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
int
ActorEntity::GetNumAnimations() const
{
    if (this->characterNode)
    {
        return this->characterNode->GetNumClips();
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    Set a new base animation. This is usually a looping animation, like
    Idle, Walking, Running, etc...

    @param  anim                new base animation
    @param  fadeIn              time to fade from current animation
    @param  timeOffset          optional animation time offset
*/
void
ActorEntity::SetBaseAnimation(const String& animName, Time fadeIn, Time timeOffset, bool onlyIfInactive, bool waitForOverlayAnim, float randomValue)
{
    n_assert(animName.IsValid());

    if (onlyIfInactive)
    {
        const String& mappedAnim = AnimTable::Instance()->GetMappedAnimName(this->animMapping, animName);    
        if (!this->baseAnimation.clipNames.IsEmpty() && this->baseAnimation.clipNames[0] == mappedAnim)
        {
            return;
        }
    }
    
    Array<String> names;
    names.Append(animName);

    Array<float> weights;
    weights.Append(1.0f);

    this->SetAnimationMix(BaseAnim, names, weights, fadeIn, timeOffset, randomValue, waitForOverlayAnim);
}

//------------------------------------------------------------------------------
/**
    Set a new overlay animation. This is usually a looping animation, like
    Idle, Walking, Running, etc...

    @param  animName            new base animation
    @param  fadeIn              time to fade from current animation
    @param  overrideDuration    if != 0.0, override the animation's duration with this value
*/
void
ActorEntity::SetOverlayAnimation(const String& animName, Time fadeIn, Time overrideDuration, bool onlyIfInactive, float randomValue)
{
    n_assert(animName.IsValid());

    if (onlyIfInactive)
    {
        const String& mappedAnim = AnimTable::Instance()->GetMappedAnimName(this->animMapping, animName);                        
        if (!this->overlayAnimation.clipNames.IsEmpty() && this->overlayAnimation.clipNames[0] == mappedAnim)
        {
            return;
        }
    }

    Array<String> names;
    names.Append(animName);

    Array<float> weights;
    weights.Append(1.0f);

    this->SetAnimationMix(OverlayAnim, names, weights, fadeIn, overrideDuration, randomValue);
}

//------------------------------------------------------------------------------
/**
    @param  clipNames           new fade animations
    @param  weights             new fade animation weights
*/
void
ActorEntity::SetFadeAnimationMix(const Array<String>& names, const Array<float>& weights, Time fadeIn, Time overrideDuration, float randomValue)
{
    n_assert(0 < names.Size());
    n_assert(weights.Size() == names.Size());

    fadeIn = 0;
    overrideDuration = 0;
    randomValue = 0;
    this->SetAnimationMix(FadeAnim, names, weights, fadeIn, overrideDuration, randomValue);
}

//------------------------------------------------------------------------------
/**
    Set an animation mix.

    @param  animType            type of animation mix
    @param  clipNames           new base / overlay animations
    @param  clipWeights         new base / overlay animation weights
    @param  fadeIn              time to fade from current animation
    @param  timingInfo          for base animations: time offset, for overlay animations: if > 0.0, override the animation's duration with this value
    @param  randomValue         random value used for animation lookup
    @param  waitForOverlayAnim  only relevant for base animations - delays base animation activation
*/
void
ActorEntity::SetAnimationMix(AnimationType animType, const Array<String>& animNames, const Array<float>& animWeights, Time fadeIn, Time timingInfo, float randomValue, bool waitForOverlayAnim)
{
    if (this->nebCharacter.isvalid())
    {
        n_assert(animNames.Size() >= 1);
        n_assert(animWeights.Size() >= 1);
        n_assert(animNames.Size() == animWeights.Size());

        // @todo: ignore if overlay animation is set and the character isn't currently visible,
        // otherwise a subtle 1-frame-bug may be triggered in ActivateAnimations() where
        // the AnimClipScheduler won't have any active clips for 1 frame
        //if ((OverlayAnim == animType) && 
        //    !(this->GetVisible() && this->GetModelInstance()))
        //{
        //    // entity is currently not visible, ignore overlay animation
        //    return;
        //}
        
        const String& mappedAnim = AnimTable::Instance()->GetMappedAnimName(this->animMapping, animNames[0]);        
        int clipIndex = this->characterNode->GetClipIndexByName(mappedAnim);
        
        if (InvalidIndex != clipIndex)
        {
            AnimationInfo* animationInfo;
            if (BaseAnim == animType)
            {
                animationInfo = &this->baseAnimation;
                animationInfo->animOffset = timingInfo;
            }
            else if (OverlayAnim == animType)
            {
                animationInfo = &this->overlayAnimation;
                animationInfo->animOffset = 0.0;
            }
            else
            {
                animationInfo = &this->fadeAnimation;
                animationInfo->animOffset = 0.0;
            }
            animationInfo->startAnim = true;

            // set fade in
            if (fadeIn < 0.0f)
            {
                //@todo: get fadein from character anim
                fadeIn = 0.2f;
            }
            animationInfo->animFadeIn = fadeIn;
            animationInfo->animStarted = this->GetTime();

            animationInfo->animNames.Clear();
            animationInfo->clipNames.Clear();
            animationInfo->clipWeights.Clear();

            animationInfo->clipNames.Append(mappedAnim);
            animationInfo->clipWeights.Append(animWeights[0]);
            animationInfo->animNames.Append(animNames[0]);
            
            // set duration
            Time duration = this->characterNode->GetClipDuration(clipIndex);
            if (timingInfo > 0.0 && OverlayAnim == animType)
            {
                animationInfo->animDuration = timingInfo;
            }
            else
            {
                Time dur = 0.0;
                if (timeFactor > 0.0f)
                {
                    dur = duration / this->timeFactor;
                }
                if (dur < 0.0)
                {
                    dur = 0.0;
                }
                animationInfo->animDuration = dur;
            }

            // append remaining clips
            IndexT animIndex;
            for (animIndex = 1; animIndex < animNames.Size(); animIndex++)
            {
                const String& animName = AnimTable::Instance()->GetMappedAnimName(this->animMapping, animNames[animIndex]);
                clipIndex = this->characterNode->GetClipIndexByName(animName );
                if (InvalidIndex != clipIndex)
                {
                    animationInfo->animNames.Append(animNames[animIndex]);
                    animationInfo->clipNames.Append(animName );
                    animationInfo->clipWeights.Append(animWeights[animIndex]);
                }
            }
        }
    }
    else
    {
        // character not loaded yet, remember requested anim
        this->requestAnim = true;
        this->curRequestedAnim.animNames = animNames;
        this->curRequestedAnim.animType = animType;
        this->curRequestedAnim.animWeights = animWeights;
        this->curRequestedAnim.fadeIn = fadeIn;
        this->curRequestedAnim.randomValue = randomValue;
        this->curRequestedAnim.timingInfo = timingInfo;
        this->curRequestedAnim.waitForOverlayAnim = waitForOverlayAnim;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ActorEntity::StopOverlayAnimation(Time fadeIn)
{
    if (0 != this->nebCharacter)
    {
        this->FadeRunningAnimationsOut(OverlayAnim);
        this->overlayAnimation.animNames.Clear();
        this->overlayAnimation.clipNames.Clear();
        this->overlayAnimation.clipWeights.Clear();

        this->baseAnimation.startAnim = true;
    }
}

//------------------------------------------------------------------------------
/**
    Fade in new base or overlay animations, fade out animations which
    are no longer active.
*/
void
ActorEntity::ActivateAnimations(AnimationType animType)
{
    if (0 != this->nebCharacter && this->nebCharacter->GetClipScheduler().isvalid())
    {        
        AnimationInfo* animationInfo = 0;
        if (BaseAnim == animType)
        {
            animationInfo = &this->baseAnimation;
        }
        else if (OverlayAnim == animType)
        {
            animationInfo = &this->overlayAnimation;
        }
        else
        {
            animationInfo = &this->fadeAnimation;
        }


        // get a copy of the currently active clips
        const Array<int> oldActiveClips = this->clipScheduler->GetActiveClipIndices();
        Array<IndexT> activatedClips;

        IndexT clipNameIndex;
        for (clipNameIndex = 0; clipNameIndex < animationInfo->clipNames.Size(); clipNameIndex++)
        {
            IndexT clipIndex = this->characterNode->GetClipIndexByName(animationInfo->clipNames[clipNameIndex]);
            if (InvalidIndex != clipIndex)
            {   
                activatedClips.Append(clipIndex);

                // set fade times
                this->clipScheduler->SetClipFadeOutTimeAt(clipIndex, (float)animationInfo->animFadeIn);
                this->clipScheduler->SetClipFadeInTimeAt(clipIndex, (float)animationInfo->animFadeIn);

                if (OverlayAnim == animType)
                {   
                    // set data for overlay clips
                    this->clipScheduler->SetClipTimeOffsetAt(clipIndex, (float)animationInfo->animOffset);
                    this->clipScheduler->SetClipRunTimeAt(clipIndex, (float)(animationInfo->animDuration / this->clipScheduler->GetClipDurationAt(clipIndex)));
                    this->clipScheduler->SetClipStartTimeAt(clipIndex, (float)animationInfo->animStarted);
                    this->clipScheduler->FadeClipAt(clipIndex, (float)animationInfo->animStarted, AnimClipScheduler::FadeIn, animationInfo->clipWeights[clipNameIndex]);
                }
                else if (FadeAnim == animType)
                {
                    // set data for overlay clips
                    this->clipScheduler->SetClipTimeOffsetAt(clipIndex, (float)animationInfo->animOffset);
                    this->clipScheduler->SetClipRunTimeAt(clipIndex, 0.0f);
                    this->clipScheduler->SetClipStartTimeAt(clipIndex, (float)animationInfo->animStarted);
                    this->clipScheduler->FadeClipAt(clipIndex, (float)animationInfo->animStarted, AnimClipScheduler::FadeIn, animationInfo->clipWeights[clipNameIndex]);
                }
                else if (BaseAnim == animType && !this->IsOverlayAnimationActive())
                {                    
                    // the first set base animation never should fade in
                    if (this->firstBaseAnim)
                    {
                        this->clipScheduler->SetClipFadeInTimeAt(clipIndex, 0.0);
                        this->firstBaseAnim = false;
                    }                    
                    
                    // do not restart base anim clips that are already running to prevent hickups
                    if ((!this->clipScheduler->IsClipActiveAt(clipIndex)) ||
                        (this->clipScheduler->GetClipFadeTypeAt(clipIndex) == AnimClipScheduler::FadeOut))
                    {
                        // set data for base clips
                        this->clipScheduler->SetClipTimeOffsetAt(clipIndex, (float)animationInfo->animOffset);
                        this->clipScheduler->SetClipRunTimeAt(clipIndex, 0.0f);
                        this->clipScheduler->SetClipStartTimeAt(clipIndex, (float)animationInfo->animStarted);
                        this->clipScheduler->FadeClipAt(clipIndex, (float)animationInfo->animStarted, AnimClipScheduler::FadeIn, animationInfo->clipWeights[clipNameIndex]);
                    }
                }
            }
        }

        if (FadeAnim != animType)
        {
            IndexT idxFadeAnim;
            for (idxFadeAnim = 0; idxFadeAnim < this->fadeAnimation.clipNames.Size(); idxFadeAnim++)
            {
                IndexT clipIndex = this->characterNode->GetClipIndexByName(this->fadeAnimation.clipNames[idxFadeAnim]);
                activatedClips.Append(clipIndex);
            }
        }
        else
        {
            if (0 < this->overlayAnimation.clipNames.Size())
            {
                IndexT clipIndex = this->characterNode->GetClipIndexByName(this->overlayAnimation.clipNames[0]);
                activatedClips.Append(clipIndex);
            }

            if (0 < this->baseAnimation.clipNames.Size())
            {
                IndexT clipIndex = this->characterNode->GetClipIndexByName(this->baseAnimation.clipNames[0]);
                activatedClips.Append(clipIndex);
            }
        }

        // fade out any previously active clips which should not continue to run
        // (and which are not already fading out)
        IndexT i;
        for (i = 0; i < oldActiveClips.Size(); i++)
        {
            IndexT clipIndex = oldActiveClips[i];
            if ((InvalidIndex == activatedClips.FindIndex(clipIndex)) && 
                (AnimClipScheduler::FadeOut !=  this->clipScheduler->GetClipFadeTypeAt(clipIndex)))
            {
                // make sure old clips fade out with the same fade time as the new clips
                // will fade in, so that we get a correct cross fade
                this->clipScheduler->SetClipFadeOutTimeAt(clipIndex, (float)animationInfo->animFadeIn);
                this->clipScheduler->FadeClipAt(clipIndex, (float)this->GetTime(), AnimClipScheduler::FadeOut);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    Call this method to commit changes of base and overlay animations.
    This method is usually only called once per frame by 
    OnRenderBefore().
*/
void
ActorEntity::CommitAnimationChanges()
{
    if (this->nebCharacter.isvalid())
    {
        // check if a new overlay animation needs to be started
        if (this->fadeAnimation.startAnim)
        {
            this->fadeAnimation.startAnim = false;
            this->ActivateAnimations(FadeAnim);
        }
        if (this->overlayAnimation.startAnim)
        {
            this->overlayAnimation.startAnim = false;
            this->ActivateAnimations(OverlayAnim);
        }
        if (this->baseAnimation.startAnim && (!this->IsOverlayAnimationActive()))
        {
            // check if a new base animation needs to be started,
            // don't start if an overlay animation is currently running
            this->baseAnimation.startAnim = false;
            this->ActivateAnimations(BaseAnim);
        }
        else if (this->IsOverlayAnimationActive())
        {
            // check if overlay animation has expired
            Time endTime = this->overlayAnimation.animStarted + this->overlayAnimation.animDuration - this->baseAnimation.animFadeIn;
            if (this->GetTime() >= endTime)
            {
                this->StopOverlayAnimation(this->baseAnimation.animFadeIn);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    This method checks whether the current overlay animation is over
    and the base animation must be re-activated. This is the only
    place where animations are actually started to prevent stuttering
    when several animations are started in the same frame.
*/
void
ActorEntity::OnUpdate()
{
    ModelEntity::OnUpdate();
    
    // validate character, for async resource loading
    this->ValidateCharacter();

    // commit any animation changes
    this->CommitAnimationChanges();

    // if character wasn't loaded on a requested anim
    if (this->requestAnim && this->nebCharacter.isvalid())
    {
        this->requestAnim = false;

        this->SetAnimationMix(this->curRequestedAnim.animType,
                              this->curRequestedAnim.animNames,
                              this->curRequestedAnim.animWeights,
                              this->curRequestedAnim.fadeIn,
                              this->curRequestedAnim.timingInfo,
                              this->curRequestedAnim.randomValue,
                              this->curRequestedAnim.waitForOverlayAnim);
    }

    //// @todo: ??? update the character's bounding box (hmm, still necessary?, this
    //// is when skins have become active)
    //if (this->HasCharacter3Set())
    //{
    //    n_assert(this->character3NodePtr);
    //    this->SetLocalBox(this->character3NodePtr->GetLocalBox());
    //}
}

//------------------------------------------------------------------------------
/**
    Returns a character joint index by its name. Returns -1 if a joint by
    that name doesn't exist in the character.
*/
int
ActorEntity::GetJointIndexByName(const String& name)
{
    n_assert(name.IsValid());
    if (0 != this->nebCharacter)
    {
        return this->nebCharacter->GetSkeleton().GetJointIndexByName(name);
    }
    else
    {
        return -1;
    }
}

//------------------------------------------------------------------------------
/**
    This brings the character's skeleton uptodate. Make sure the
    entity's time and animation state weights in the rendercontext are uptodate
    before calling this method, to avoid one-frame-latencies.
*/
void
ActorEntity::EvaluateSkeleton(bool enforceEvaluation)
{
    if (0 != this->nebCharacter)
    {
        this->CommitAnimationChanges();
        float time = (float) this->GetTime();
        if (this->characterSet != 0 && this->characterSet->IsVariationDirty())
        {
            this->characterSet->ApplyCurrentVariation();
        }
        this->nebCharacter->EvaluateSkeleton(time, enforceEvaluation);
    }
}

//------------------------------------------------------------------------------
/**
    Return pointer to joint or 0 if joint doesn't exist. 
*/
CharJoint*
ActorEntity::GetJoint(int jointIndex) const
{
    if (0 != this->nebCharacter)
    {
        return &(this->nebCharacter->GetSkeleton().GetJointAt(jointIndex));
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
    Returns a joint's current matrix in model space. Make sure to call
    EvaluateSkeleton() before!
*/
const matrix44&
ActorEntity::GetLocalJointMatrix(int jointIndex) const
{
    if (0 != this->nebCharacter)
    {
        return this->nebCharacter->GetSkeleton().GetJointAt(jointIndex).GetMatrix();
    }
    else
    {
        static matrix44 identM = matrix44::identity();
        return identM;
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
ActorEntity::SetupCharacter3Set()
{
    n_assert(0 != this->characterNode);

    // get the character set from characternodeinstance
    const Ptr<Models::ModelInstance>& modelInstance = this->GetModelInstance();
    const Ptr<Models::ModelNodeInstance>& nodeInst = modelInstance->LookupNodeInstanceByRTTI(Models::CharacterNodeInstance::RTTI);
    n_assert(nodeInst->IsA(Models::CharacterNodeInstance::RTTI));
    const Ptr<Models::CharacterNodeInstance>& charNodeInst = nodeInst.downcast<Models::CharacterNodeInstance>();        
    this->characterSet = charNodeInst->GetCharacterSet();    
    n_assert(this->characterSet.isvalid());

    // load from file
    if (this->skinList.IsValid())
    {
        characterSet->SetSkinListFileName(this->skinList);
        characterSet->LoadSkinList();
    }    
}

//------------------------------------------------------------------------------
/**
    Find the joint matrix in world space by index. Careful, this method
    does an EvaluateSkeleton() per invocation!
*/
matrix44
ActorEntity::GetJointMatrixByIndex(IndexT jointIndex)
{
    n_assert2(InvalidIndex != jointIndex, "Invalid joint index");
    matrix44 jointMatrix;

    // update Nebula character skeleton
    this->EvaluateSkeleton();

    // get my entity's transformation
    const matrix44& worldMatrix = this->GetTransform();

    // a correctional matrix which rotates the joint matrix by 180
    // degree around global Y to account for the fact the Nebula2
    // characters look along +Z, not -Z
    matrix44 rot180 = matrix44::rotationy(n_deg2rad(180.0f));

    jointMatrix = this->GetLocalJointMatrix(jointIndex);
    jointMatrix = matrix44::multiply(jointMatrix, rot180);
    jointMatrix = matrix44::multiply(jointMatrix, worldMatrix);

    return jointMatrix;
}

//------------------------------------------------------------------------------
/**
    Returns a joint matrix in world space by name. Careful, this method
    does an EvaluateSkeleton() per invocation!
*/
matrix44
ActorEntity::GetJointMatrixByName(const String& jointName)
{
    n_assert(jointName.IsValid());

    // find the index of the joint
    int jointIndex = this->GetJointIndexByName(jointName);
    if (jointIndex == InvalidIndex)
    {
        n_error("Joint index for joint with name '%s' not found", jointName.AsCharPtr());
    }

    return this->GetJointMatrixByIndex(jointIndex);
}

//------------------------------------------------------------------------------
/**
    Fades an animation to the specified target weight

    @param  animRestart     enable this, if you want to restart the clip (if it runs already) before fading it to the target weight 
    @param  fadeOutRunningAnims     enable this, if all currently running clips should fade out
*/
void
ActorEntity::FadeAnimation(const String& anim, float targetWeight, Time fadeInTime, Time fadeOutTime, float randomValue, float runTime, float timeFactor, Time sampleTime, bool animRestart, bool fadeOutRunningAnims)
{
    n_assert(anim.IsValid());

    Array<String> anims;
    anims.Append(anim);
    
    this->FadeAnimationMix(anims, targetWeight, fadeInTime, fadeOutTime, randomValue, runTime, timeFactor, sampleTime, animRestart, fadeOutRunningAnims);
}

//------------------------------------------------------------------------------
/**
    Fades a mix of animations to the specified target weight

    @param  animRestart     enable this, if you want to restart currently running clips before fading them to the target weight 
    @param  fadeOutRunningAnims     enable this, if all currently running clips should fade out
*/
void
ActorEntity::FadeAnimationMix(const Array<String>& anims, float targetWeight, Time fadeInTime, Time fadeOutTime, float randomValue, float runTime, float timeFactor, Time sampleTime, bool animRestart, bool fadeOutRunningAnims)
{
    n_assert(anims.Size() >= 1);
    
    Array<float> weights;
    weights.Append(targetWeight);

    this->FadeAnimationMix(anims, weights, fadeInTime, fadeOutTime, randomValue, runTime, timeFactor, sampleTime, animRestart, fadeOutRunningAnims);    
}

//------------------------------------------------------------------------------
/**
    Fades a mix of animations to the specified target weights

    @param  animRestart     enable this, if you want to restart currently running clips before fading them to the target weights
    @param  fadeOutRunningAnims     enable this, if all currently running clips should fade out
*/
void
ActorEntity::FadeAnimationMix(const Array<String>& anims, const Array<float>& targetWeights, Time fadeInTime, Time fadeOutTime, float randomValue, float runTime, float timeFactor, Time sampleTime, bool animRestart, bool fadeOutRunningAnims)
{
    n_assert(anims.Size() >= 1);
    
    if(fadeOutRunningAnims)
    {
        this->FadeRunningAnimationsOut();
    }

    AnimClipScheduler::FadeType fadeType = AnimClipScheduler::NoFade;
    float currentWeight;
    int clipIndex;
    IndexT animIndex;
    for(animIndex = 0; animIndex < anims.Size(); animIndex++)
    {
        String mappedAnimName = AnimTable::Instance()->GetMappedAnimName(this->animMapping, anims[animIndex]);
        clipIndex = this->characterNode->GetClipIndexByName(mappedAnimName);
        
        this->clipScheduler->SetClipRunTimeAt(clipIndex, runTime);
        this->clipScheduler->SetClipTimeFactorAt(clipIndex, timeFactor);
        this->clipScheduler->SetClipSampleTimeAt(clipIndex, (float)sampleTime);
        this->clipScheduler->SetClipFadeInTimeAt(clipIndex, (float)fadeInTime);
        this->clipScheduler->SetClipFadeOutTimeAt(clipIndex, (float)fadeOutTime);

        // if a clip should be (re-)started, set a new start time
        if(animRestart || !this->clipScheduler->IsClipActiveAt(clipIndex))
        {
            this->clipScheduler->SetClipStartTimeAt(clipIndex, (float)this->GetTime());
        }
        
        currentWeight = this->clipScheduler->GetClipWeightAt(clipIndex);
        
        // choose a fade type - depending on current weight
        if(currentWeight < targetWeights[animIndex])
        {
            fadeType = AnimClipScheduler::FadeIn;
        }
        else if(currentWeight > targetWeights[animIndex])
        {
            fadeType = AnimClipScheduler::FadeOut;
        }

        if(AnimClipScheduler::NoFade != fadeType)
        {
            this->clipScheduler->FadeClipAt(clipIndex, (float)this->GetTime(), fadeType, targetWeights[animIndex]);      
        }
    }
}

//------------------------------------------------------------------------------
/**
    Fade out all animation clips with a weight > 0.0f
*/
void
ActorEntity::FadeRunningAnimationsOut()
{
    IndexT clipIndex;
    for (clipIndex = 0; clipIndex < (uint)this->clipScheduler->GetNumClips(); clipIndex++)
    {
        if (this->clipScheduler->IsClipActiveAt(clipIndex))
        {
            this->clipScheduler->FadeClipAt(clipIndex, (float)this->GetTime(), AnimClipScheduler::FadeOut);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Fade out all animation clips with a weight > 0.0f, if they are running as Base- or Overlay-Animation

    @param  type    specify which animation type should fade out (Base- or Overlay-Animations)
*/
void
ActorEntity::FadeRunningAnimationsOut(AnimationType type, const Array<int>* clipFilter)
{
    n_assert(0 != this->nebCharacter);
    
    Array<String>* clipNames = 0;
    
    // set clip name array pointer depending on AnimationType
    if(BaseAnim == type)
    {
        clipNames = &this->baseAnimation.clipNames;
    }
    else
    {
        clipNames = &this->overlayAnimation.clipNames;
    }
    n_assert(0 != clipNames);

    // fade running anims out
    int clipIndex;

    IndexT animIndex;
    for(animIndex = 0; animIndex < clipNames->Size(); animIndex++)
    {
        clipIndex = this->characterNode->GetClipIndexByName(clipNames->At(animIndex));

        // FIXME: special case handling: base and overlay animation share one or more clips
        // fade out, if the clip filter is not set or does not contain the current clip index
        if(0 == clipFilter || InvalidIndex == clipFilter->FindIndex(clipIndex))
        {
            this->clipScheduler->FadeClipAt(clipIndex, (float)this->GetTime(), AnimClipScheduler::FadeOut);      
        }
    }
}

//------------------------------------------------------------------------------
/**
    FIXME: special case handling: base and overlay animation share one or more clips
*/
bool
ActorEntity::IsClipUsedByBaseAnimation(int clipIndex)
{

    if(InvalidIndex != this->baseAnimation.clipNames.FindIndex(this->clipScheduler->GetClipNameAt(clipIndex)))
    {
        return true;
    }
    
    return false;   
}

} // namespace Graphics
