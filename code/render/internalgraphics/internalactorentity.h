#pragma once
#ifndef INTERNALGRAPHICS_INTERNALACTORENTITY_H
#define INTERNALGRAPHICS_INTERNALACTORENTITY_H
//------------------------------------------------------------------------------
/**
    @class InternalGraphics::InternalActorEntity
  
    An actor graphics entity. NOTE: The current InternalActorEntity class is
    just a wrapper around the legacy Nebula2 Character3 system and will
    be replaced with a "proper" Nebula3 character system in the future.

    (C) 2007 Radon Labs GmbH
*/    
#include "internalgraphics/internalmodelentity.h"
#include "models/nodes/characternode.h"
#include "nebula2/nebula2wrapper.h"

//------------------------------------------------------------------------------
namespace InternalGraphics
{
class InternalActorEntity : public InternalModelEntity
{
    __DeclareClass(InternalActorEntity);
public:
    enum AnimationType
    {
        BaseAnim,
        OverlayAnim,
        FadeAnim
    };

    /// constructor
    InternalActorEntity();
    /// destructor
    virtual ~InternalActorEntity();
    /// called when attached to game entity
    virtual void OnActivate();
    /// called when removed from game entity
    virtual void OnDeactivate();
    /// set animation mapping to use
    void SetAnimationMapping(const Util::String& n);
    /// get animation mapping to use
    const Util::String& GetAnimationMapping() const;
    /// set optional skin lists for character3's
    void SetSkinList(const Util::String& s);
    /// get skin list
    const Util::String& GetSkinList() const;
    /// set the current base animation (usually loop anims) - OBSOLETE, use FadeAnimation(), ClearAnimations()
    void SetBaseAnimation(const Util::String& anim, Timing::Time fadeIn = 0.0, Timing::Time timeOffset = 0.0, bool onlyIfInactive = true, bool waitForOverlayAnim = false, float randomValue = 0.0f);
    /// get current base animation (first of mix)
    Util::String GetBaseAnimation() const;
    /// get current base clip (first of mix)
    Util::String GetBaseClip() const;
    /// get base animation duration
    Timing::Time GetBaseAnimationDuration() const;
    /// set the current overlay animation (usually oneshot anims) - OBSOLETE, use FadeAnimation(), ClearAnimations()
    void SetOverlayAnimation(const Util::String& anim, Timing::Time fadeIn = 0.0, Timing::Time overrideDuration = 0.0, bool onlyIfInactive = true, float randomValue = 0.0f);
    /// get name of currently active overlay animation (first of mix)
    Util::String GetOverlayAnimation() const;
    /// get name of currently active overlay clip (first of mix)
    Util::String GetOverlayClip() const;
    /// get overlay animation duration
    Timing::Time GetOverlayAnimationDuration() const;
    /// set the current fade animations - OBSOLETE, use FadeAnimation(), ClearAnimations()
    void SetFadeAnimationMix(const Util::Array<Util::String>& anims, const Util::Array<float>& weights, Timing::Time fadeIn, Timing::Time overrideDuration, float randomValue);

    /// stop current overlay animation
    void StopOverlayAnimation(Timing::Time fadeIn);
    /// return true if overlay animation is currently active
    bool IsOverlayAnimationActive() const;
    /// called before rendering happens
    virtual void OnUpdate();
    /// get pointer to our nCharacter2 object
    Char::Character* GetCharacterPointer() const;
    /// bring the character's skeleton uptodate
    void EvaluateSkeleton(bool enforceEvaluation = false);
    /// is this a character3 ?
    bool HasCharacter3Set() const;
    /// get the character3 set, fail if no character3 set exists
    Char::CharacterSet* GetCharacter3Set() const;    

    /// return joint index by name, return InvalidIndex if joint doesn't exist
    int GetJointIndexByName(const Util::String& name);
    /// return joint at index
    Char::CharJoint* GetJoint(int jointIndex) const;
    /// return a joint's current matrix in model space
    const Math::matrix44& GetLocalJointMatrix(int jointIndex) const;
    /// get joint matrix by index in global space
    Math::matrix44 GetJointMatrixByIndex(IndexT jointIndex);
    /// get joint matrix by name in global space, throws assert if joint doesn't exist
    Math::matrix44 GetJointMatrixByName(const Util::String& jointName);
    
    /// return the animation clip scheduler
    Anim::AnimClipScheduler* GetAnimClipScheduler() const;
    /// fade an animation to a weight
    void FadeAnimation(const Util::String& anim, float targetWeight = 0.0f, Timing::Time fadeInTime = 0.0, Timing::Time fadeOutTime = 0.0, float randomValue = 0.0f, float runTime = 0.0f, float timeFactor = 1.0f, Timing::Time sampleTime = 0.0, bool animRestart = false, bool fadeOutRunningAnims = true);
    /// fade a mix of animations to a weight
    void FadeAnimationMix(const Util::Array<Util::String>& anims, float targetWeight = 0.0f, Timing::Time fadeInTime = 0.0, Timing::Time fadeOutTime = 0.0, float randomValue = 0.0f, float runTime = 0.0f, float timeFactor = 1.0f, Timing::Time sampleTime = 0.0, bool animRestart = false, bool fadeOutRunningAnims = true);
    /// fade a mix of animations to various weights
    void FadeAnimationMix(const Util::Array<Util::String>& anims, const Util::Array<float>& targetWeights, Timing::Time fadeInTime = 0.0, Timing::Time fadeOutTime = 0.0, float randomValue = 0.0f, float runTime = 0.0f, float timeFactor = 1.0f, Timing::Time sampleTime = 0.0, bool animRestart = false, bool fadeOutRunningAnims = true);
    /// fade all running animations out
    void FadeRunningAnimationsOut();
    /// fades running clips of the given type out
    void FadeRunningAnimationsOut(AnimationType type, const Util::Array<int>* clipFilter = 0);

    /// get character node
    const Ptr<Models::CharacterNode>& GetCharacterNode() const;

private:
    /// actually commit animation changes
    void CommitAnimationChanges();
    /// activate animations
    void ActivateAnimations(AnimationType animType);
    /// sets a base or overlay animation mix
    void SetAnimationMix(AnimationType animType, const Util::Array<Util::String>& anims, const Util::Array<float>& weights, Timing::Time fadeIn = 0.0, Timing::Time timingInfo = 0.0, float randomValue = 0.0f, bool waitForOverlayAnim = false);
    /// get the number of animations of this character
    int GetNumAnimations() const;
    /// set the character 3 set to load
    void SetupCharacter3Set();
    /// Check if the given clip index is used for the current base animations (used to determine special case handling: overlay and base share the same anim clip)
    bool IsClipUsedByBaseAnimation(int clipIndex);
    /// validate character
    void ValidateCharacter();

    Util::String animMapping;                ///< name of the animation mapping
    Util::String skinList;                   ///< optional skin list for character3's
    bool firstBaseAnim;
    
    Ptr<Char::Character> nebCharacter;
    Ptr<Char::CharacterSet> characterSet;
    Ptr<Anim::AnimClipScheduler> clipScheduler;
    Ptr<Models::CharacterNode> characterNode;

    bool character3Mode;
    
    struct AnimationInfo
    {
        bool startAnim;

        Util::Array<Util::String> animNames;
        Util::Array<Util::String> clipNames;
        Util::Array<float> clipWeights;

        Timing::Time animDuration;
        Timing::Time animStarted;
        Timing::Time animFadeIn;
        Timing::Time animOffset;
    };

    struct RequestAnimEntry
    {       
        AnimationType animType;
        Util::Array<Util::String> animNames;
        Util::Array<float> animWeights;
        Timing::Time fadeIn;
        Timing::Time timingInfo;
        float randomValue;
        bool waitForOverlayAnim;
    };
    
    AnimationInfo baseAnimation;
    AnimationInfo overlayAnimation;
    AnimationInfo fadeAnimation; 
    RequestAnimEntry curRequestedAnim;
    bool requestAnim;
    float timeFactor;
};

//------------------------------------------------------------------------------
/**
*/
inline void
InternalActorEntity::SetAnimationMapping(const Util::String& name)
{
    this->animMapping = name;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
InternalActorEntity::GetAnimationMapping() const
{
    return this->animMapping;
}

//------------------------------------------------------------------------------
/**
*/
inline void
InternalActorEntity::SetSkinList(const Util::String& s)
{
    this->skinList = s;
}
//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
InternalActorEntity::GetSkinList() const
{
    return this->skinList;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalActorEntity::IsOverlayAnimationActive() const
{
    return !this->overlayAnimation.animNames.IsEmpty();
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
InternalActorEntity::GetOverlayAnimation() const
{
    return this->overlayAnimation.animNames.IsEmpty() ? "" : this->overlayAnimation.animNames[0];
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
InternalActorEntity::GetBaseAnimation() const
{
    return this->baseAnimation.animNames.IsEmpty() ? "" : this->baseAnimation.animNames[0];
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
InternalActorEntity::GetOverlayClip() const
{
    return this->overlayAnimation.clipNames.IsEmpty() ? "" : this->overlayAnimation.clipNames[0];
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
InternalActorEntity::GetBaseClip() const
{
    return this->baseAnimation.clipNames.IsEmpty() ? "" : this->baseAnimation.clipNames[0];
}


//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
InternalActorEntity::GetOverlayAnimationDuration() const
{
    return this->overlayAnimation.animDuration;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
InternalActorEntity::GetBaseAnimationDuration() const
{
    return this->baseAnimation.animDuration;
}

//------------------------------------------------------------------------------
/**
    Returns the pointer to our nCharacter2 object.
*/
inline Char::Character*
InternalActorEntity::GetCharacterPointer() const
{
    return this->nebCharacter;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InternalActorEntity::HasCharacter3Set() const
{
    return (0 != this->characterSet);
}

//------------------------------------------------------------------------------
/**
*/
inline Char::CharacterSet*
InternalActorEntity::GetCharacter3Set() const
{
    n_assert(this->HasCharacter3Set());
    return this->characterSet;
}

//------------------------------------------------------------------------------
/**
*/
inline Anim::AnimClipScheduler*
InternalActorEntity::GetAnimClipScheduler() const
{
    n_assert(0 != this->nebCharacter);
    return this->nebCharacter->GetClipScheduler();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Models::CharacterNode>& 
InternalActorEntity::GetCharacterNode() const
{
    return this->characterNode;
}
} // namespace InternalGraphics
//------------------------------------------------------------------------------
#endif
