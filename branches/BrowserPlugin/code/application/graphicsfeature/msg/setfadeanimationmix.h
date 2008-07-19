#ifndef MSG_SETFADEANIMATIONMIX_H
#define MSG_SETFADEANIMATIONMIX_H
//------------------------------------------------------------------------------
/**
    @class Messaging::SetFadeAnimationMix
    
    Set fade animation on an actor.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class SetFadeAnimationMix : public Messaging::Message
{
    DeclareClass(SetFadeAnimationMix);
    DeclareMsgId;

public:
    /// constructor
    SetFadeAnimationMix();

    /// set optional fade in time
    void SetFadeInTime(Timing::Time t);
    /// get optional fade in time
    Timing::Time GetFadeInTime() const;

    /// set optional run time
    void SetRunTime(float t);
    /// get optional run time
    float GetRunTime() const;

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
    float runTime;
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
SetFadeAnimationMix::SetFadeAnimationMix() :
    fadeInTime(0.0),
    runTime(0.0f),
    randomValue(0.0f),
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
SetFadeAnimationMix::SetAnimationMix(const Util::Array<Util::String>& n, const Util::Array<float>& w, bool restartAnims, bool fadeOutRunningAnims)
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
SetFadeAnimationMix::GetAnimationMixNames() const
{
    return this->anims;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::Array<float>&
SetFadeAnimationMix::GetAnimationMixWeights() const
{
    return this->weights;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetFadeAnimationMix::SetFadeInTime(Timing::Time t)
{
    this->fadeInTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
SetFadeAnimationMix::GetFadeInTime() const
{
    return this->fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetFadeAnimationMix::SetRunTime(float t)
{
    this->runTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
SetFadeAnimationMix::GetRunTime() const
{
    return this->runTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
SetFadeAnimationMix::SetRandomValue(float random)
{
    this->randomValue = random;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
SetFadeAnimationMix::GetRandomValue() const
{
    return this->randomValue;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
SetFadeAnimationMix::IsRestartAnimSet() const
{
    return this->restartAnim;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
SetFadeAnimationMix::IsFadeOutRunningAnimsSet() const
{
    return this->fadeOutRunningAnims;
}

}; // namespace Msg
//------------------------------------------------------------------------------
#endif
