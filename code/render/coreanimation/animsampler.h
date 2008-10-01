#pragma once
#ifndef COREANIMATION_ANIMSAMPLER_H
#define COREANIMATION_ANIMSAMPLER_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimSampler
  
    An AnimSample object computes and stored the current sampled snapshot of an 
    animation clip at some point in time as one float4 key per clip-curve.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"
#include "coreanimation/animclip.h"
#include "coreanimation/sampletype.h"
#include "coreanimation/animresource.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class AnimSampler
{
public:
    /// constructor
    AnimSampler();
    /// destructor
    ~AnimSampler();
    
    /// setup the object
    void Setup(const Ptr<AnimResource>& animResource);
    /// discard the anim sampler
    void Discard();
    /// return true if object has been setup
    bool IsValid() const;

    /// sample an animation clip at some point in time
    void Sample(IndexT clipIndex, SampleType::Code sampleType, Timing::Tick time);
    /// get number of keys in the result
    SizeT GetNumKeys() const;
    /// get pointer to result samples
    Math::float4* GetSamples() const;
    /// get number of samples which contribute to the result
    uchar* GetSampleCounts() const;

private:
    Ptr<AnimResource> animResource;
    SizeT numKeys;
    Math::float4* sampleBuffer;
    uchar* sampleCounts;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
AnimSampler::IsValid() const
{
    return this->animResource.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
AnimSampler::GetNumKeys() const
{
    return this->numKeys;
}

//------------------------------------------------------------------------------
/**
*/
inline Math::float4*
AnimSampler::GetSamples() const
{
    n_assert(this->IsValid());
    return this->sampleBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline uchar*
AnimSampler::GetSampleCounts() const
{
    return this->sampleCounts;
}

} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif
