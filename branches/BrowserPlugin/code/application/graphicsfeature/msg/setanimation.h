#ifndef MSG_SETANIMATION_H
#define MSG_SETANIMATION_H
//------------------------------------------------------------------------------
/**
    @class Messaging::SetAnimation
    
    Set base or overlay animation on an actor.
    
    (C) 2005 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class SetAnimation : public Messaging::Message
{
    DeclareClass(SetAnimation);
    DeclareMsgId;

public:
    /// constructor
    SetAnimation();

    /// set optional fade in time
    void SetFadeInTime(Timing::Time t);
    /// get optional fade in time
    Timing::Time GetFadeInTime() const;

    /// set base animation
    void SetBaseAnimation(const Util::String& n);
    /// get base animation
    const Util::String& GetBaseAnimation() const;

    /// set mixed base animation names and weights
    void SetMixedBaseAnimation(const Util::Array<Util::String>& n, const Util::Array<float>& w);
    /// get mixed base animation names
    const Util::Array<Util::String>& GetMixedBaseAnimationNames() const;
    /// get mixed base animation weights
    const Util::Array<float>& GetMixedBaseAnimationWeights() const;

    /// set optional base animation time offset
    void SetBaseAnimTimeOffset(Timing::Time t);
    /// get base animation time offset
    Timing::Time GetBaseAnimTimeOffset() const;

    /// set overlay animation (override base anim)
    void SetOverlayAnimation(const Util::String& n);
    /// get overlay animation
    const Util::String& GetOverlayAnimation() const;

    /// set mixed overlay animation names and weights
    void SetMixedOverlayAnimation(const Util::Array<Util::String>& n, const Util::Array<float>& w);
    /// get mixed overlay animation names
    const Util::Array<Util::String>& GetMixedOverlayAnimationNames() const;
    /// get mixed overlay animation weights
    const Util::Array<float>& GetMixedOverlayAnimationWeights() const;

    /// set optional overlay anim duration override
    void SetOverlayAnimDurationOverride(Timing::Time t);
    /// get optional overlay anim duration override
    Timing::Time GetOverlayAnimDurationOverride() const;
    /// stop current overlay animation
    void SetOverlayAnimStop(bool b);
    /// get overlay anim stop flag
    bool GetOverlayAnimStop() const;

    /// set optional random value
    void SetRandomValue(float random);
    /// get optional random value
    float GetRandomValue() const;


private:
    Timing::Time fadeInTime;
    Util::String baseAnim;
    Util::String overlayAnim;
    Timing::Time baseAnimTimeOffset;
    Timing::Time overlayAnimDurationOverride;
    bool stopOverlayAnim;

    float randomValue;

    Util::Array<Util::String> mixedBaseAnimNames;
    Util::Array<float> mixedBaseAnimWeights;
    Util::Array<Util::String> mixedOverlayAnimNames;
    Util::Array<float> mixedOverlayAnimWeights;
};

//------------------------------------------------------------------------------
/**
*/
inline
SetAnimation::SetAnimation() :
    fadeInTime(-0.2),
    baseAnimTimeOffset(0.0),
    overlayAnimDurationOverride(0.0),
    stopOverlayAnim(false),
    randomValue(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetAnimation::SetBaseAnimation(const Util::String& n)
{
    this->baseAnim = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
SetAnimation::GetBaseAnimation() const
{
    return this->baseAnim;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetAnimation::SetMixedBaseAnimation(const Util::Array<Util::String>& n, const Util::Array<float>& w)
{
    this->mixedBaseAnimNames = n;
    this->mixedBaseAnimWeights = w;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::Array<Util::String>&
SetAnimation::GetMixedBaseAnimationNames() const
{
    return this->mixedBaseAnimNames;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::Array<float>&
SetAnimation::GetMixedBaseAnimationWeights() const
{
    return this->mixedBaseAnimWeights;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetAnimation::SetFadeInTime(Timing::Time t)
{
    this->fadeInTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
SetAnimation::GetFadeInTime() const
{
    return this->fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetAnimation::SetBaseAnimTimeOffset(Timing::Time t)
{
    this->baseAnimTimeOffset = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
SetAnimation::GetBaseAnimTimeOffset() const
{
    return this->baseAnimTimeOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetAnimation::SetOverlayAnimation(const Util::String& n)
{
    this->overlayAnim = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
SetAnimation::GetOverlayAnimation() const
{
    return this->overlayAnim;
}


//------------------------------------------------------------------------------
/**
*/
inline
void
SetAnimation::SetMixedOverlayAnimation(const Util::Array<Util::String>& n, const Util::Array<float>& w)
{
    this->mixedOverlayAnimNames = n;
    this->mixedOverlayAnimWeights = w;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::Array<Util::String>&
SetAnimation::GetMixedOverlayAnimationNames() const
{
    return this->mixedOverlayAnimNames;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::Array<float>&
SetAnimation::GetMixedOverlayAnimationWeights() const
{
    return this->mixedOverlayAnimWeights;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetAnimation::SetOverlayAnimDurationOverride(Timing::Time t)
{
    this->overlayAnimDurationOverride = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
SetAnimation::GetOverlayAnimDurationOverride() const
{
    return this->overlayAnimDurationOverride;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetAnimation::SetOverlayAnimStop(bool b)
{
    this->stopOverlayAnim = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
SetAnimation::GetOverlayAnimStop() const
{
    return this->stopOverlayAnim;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetAnimation::SetRandomValue(float random)
{
    this->randomValue = random;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
SetAnimation::GetRandomValue() const
{
    return this->randomValue;
}

}; // namespace Msg
//------------------------------------------------------------------------------
#endif
