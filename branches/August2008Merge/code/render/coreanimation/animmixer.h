#pragma once
#ifndef COREANIMATION_ANIMMIXER_H
#define COREANIMATION_ANIMMIXER_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimMixer
    
    The AnimMixer mixes the content of exactly 2 source animation samplers
    into a result animation sampler. AnimMixer objects are used by
    AnimBlendTree to perform one blend operation in the blend tree.
    The sample counts of the destination animation sampler indicate
    how many active animation curves have contributed to the result
    (this is the sum of active animation curves which have been
    mixed into the resulting sample buffer).

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsampler.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class AnimMixer
{
public:
    /// constructor
    AnimMixer();
    /// destructor
    ~AnimMixer();

    /// setup the animation mixer
    void Setup(const Ptr<AnimResource>& animResource);
    /// discard the animation mixer
    void Discard();
    /// return true if the animation mixer has been setup
    bool IsValid() const;

    /// mix 2 source anim samplers into a destination sampler: dst = lerp(src0, src1, lerpValue)
    void Mix(const AnimSampler& src0, const AnimSampler& src1, float lerpValue, const AnimSampler& dst);

private:
    Ptr<AnimResource> animResource;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
AnimMixer::IsValid() const
{
    return this->animResource.isvalid();
}

} // namespace AnimMixer
//------------------------------------------------------------------------------
#endif
