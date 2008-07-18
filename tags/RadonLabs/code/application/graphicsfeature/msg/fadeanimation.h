#ifndef MSG_FADEANIMATION_H
#define MSG_FADEANIMATION_H
//------------------------------------------------------------------------------
/**
    @class Messaging::FadeAnimation
    
    Fade an animation on an actor.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class FadeAnimation : public Messaging::Message
{
    DeclareClass(FadeAnimation);
    DeclareMsgId;

public:
    /// constructor
    FadeAnimation();

    /// set optional fade in time
    void SetFadeInTime(Timing::Time t);
    /// get optional fade in time
    Timing::Time GetFadeInTime() const;

    /// set optional fade out time
    void SetFadeOutTime(Timing::Time t);
    /// get optional fade out time
    Timing::Time GetFadeOutTime() const;

    /// set optional run time
    void SetRunTime(float t);
    /// get optional run time
    float GetRunTime() const;

    /// set optional sample time
    void SetSampleTime(Timing::Time t);
    /// get optional sample time
    Timing::Time GetSampleTime() const;

    /// set optional time factor
    void SetTimeFactor(float t);
    /// get optional time factor
    float GetTimeFactor() const;

    /// set animation name and weight
    void SetAnimation(const Util::String& n, float w, bool restartAnim = false, bool fadeOutRunningAnims = true);
    /// get animation name
    const Util::String& GetAnimationName() const;
    /// get animation weight
    float GetAnimationWeight() const;

    /// set animation mix names and weights
    void SetAnimationMix(const Util::Array<Util::String>& n, float w, bool restartAnims = false, bool fadeOutRunningAnims = true);
    /// get animation mix names
    const Util::Array<Util::String>& GetAnimationMixNames() const;
    /// set animation mix names and weights
    void SetAnimationMix(const Util::Array<Util::String>& n, const Util::Array<float>& w, bool restartAnims = false, bool fadeOutRunningAnims = true);
    /// get animation mix weights
    const Util::Array<float>& GetAnimationMixWeights() const;

    // set optional random value
    void SetRandomValue(float random);
    /// get optional random value
    float GetRandomValue() const;

    /// get optional restart anim flag
    bool IsRestartAnimSet() const;
    /// get optional fade out running anims flag
    bool IsFadeOutRunningAnimsSet() const;

private:
    Timing::Time fadeInTime;
    Timing::Time fadeOutTime;
    Timing::Time sampleTime;
    Util::String anim;
    float weight;
    float runTime;
    float timeFactor;
    float randomValue;
    bool restartAnim;
    bool fadeOutRunningAnims;

    Util::Array<Util::String> anims;
    Util::Array<float> weights;
};

//------------------------------------------------------------------------------
/**
*/
inline
FadeAnimation::FadeAnimation() :
    fadeInTime(0.2),
    fadeOutTime(0.2),
    runTime(0.0f),
    randomValue(0.0f),
    weight(0.0f),
    sampleTime(0.0),
    timeFactor(1.0f),
    restartAnim(false),
    fadeOutRunningAnims(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
FadeAnimation::SetAnimation(const Util::String& n, float w, bool restartAnim, bool fadeOutRunningAnims)
{
    this->anim = n;
    this->weight = w;
    this->restartAnim = restartAnim;
    this->fadeOutRunningAnims = fadeOutRunningAnims;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
FadeAnimation::GetAnimationName() const
{
    return this->anim;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
FadeAnimation::GetAnimationWeight() const
{
    return this->weight;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
FadeAnimation::SetAnimationMix(const Util::Array<Util::String>& n, float w, bool restartAnims, bool fadeOutRunningAnims)
{
    this->anims = n;
    this->weight = w;
    this->restartAnim = restartAnims;
    this->fadeOutRunningAnims = fadeOutRunningAnims;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
FadeAnimation::SetAnimationMix(const Util::Array<Util::String>& n, const Util::Array<float>& w, bool restartAnims, bool fadeOutRunningAnims)
{
    this->anims = n;
    this->weights = w;
    this->restartAnim = restartAnims;
    this->fadeOutRunningAnims = fadeOutRunningAnims;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::Array<Util::String>&
FadeAnimation::GetAnimationMixNames() const
{
    return this->anims;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::Array<float>&
FadeAnimation::GetAnimationMixWeights() const
{
    return this->weights;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
FadeAnimation::SetFadeInTime(Timing::Time t)
{
    this->fadeInTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
FadeAnimation::GetFadeInTime() const
{
    return this->fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
FadeAnimation::SetFadeOutTime(Timing::Time t)
{
    this->fadeOutTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
FadeAnimation::GetFadeOutTime() const
{
    return this->fadeOutTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
FadeAnimation::SetRunTime(float t)
{
    this->runTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
FadeAnimation::GetRunTime() const
{
    return this->runTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
FadeAnimation::SetSampleTime(Timing::Time t)
{
    this->sampleTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
FadeAnimation::GetSampleTime() const
{
    return this->sampleTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
FadeAnimation::SetTimeFactor(float t)
{
    this->timeFactor = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
FadeAnimation::GetTimeFactor() const
{
    return this->timeFactor;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
FadeAnimation::SetRandomValue(float random)
{
    this->randomValue = random;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
FadeAnimation::GetRandomValue() const
{
    return this->randomValue;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
FadeAnimation::IsRestartAnimSet() const
{
    return this->restartAnim;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
FadeAnimation::IsFadeOutRunningAnimsSet() const
{
    return this->fadeOutRunningAnims;
}

}; // namespace Msg
//------------------------------------------------------------------------------
#endif
