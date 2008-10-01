#pragma once
#ifndef COREANIMATION_ANIMSAMPLEBUFFER_H
#define COREANIMATION_ANIMSAMPLEBUFFER_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimSampleBuffer
  
    Stores the result of an animation sampling operation and keeps count
    on the number of source samples which contributed to a resulting
    sample. AnimSampleBuffer are used inside of AnimTrees to store
    and transfer the result of sampling and blending operations.
     
    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "coreanimation/animresource.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class AnimSampleBuffer : public Core::RefCounted
{
    DeclareClass(AnimSampleBuffer);
public:
    /// constructor
    AnimSampleBuffer();
    /// destructor
    virtual ~AnimSampleBuffer();
    
    /// setup the object from an animation resource
    void Setup(const Ptr<AnimResource>& animResource);
    /// discard the object
    void Discard();
    /// return true if the object has been setup
    bool IsValid() const;
    
    /// get the number of samples in the buffer
    SizeT GetNumSamples() const;
    /// get access to the sample buffer
    Math::float4* GetSampleValues() const;
    /// get access to the sample counts
    uchar* GetSampleCounts() const;

private:
    Ptr<AnimResource> animResource;
    SizeT numSamples;
    Math::float4* sampleValues;
    uchar* sampleCounts;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
AnimSampleBuffer::IsValid() const
{
    return this->animResource.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
AnimSampleBuffer::GetNumSamples() const
{
    return this->numSamples;
}

//------------------------------------------------------------------------------
/**
*/
inline Math::float4*
AnimSampleBuffer::GetSampleValues() const
{
    return this->sampleValues;
}

//------------------------------------------------------------------------------
/**
*/
inline uchar*
AnimSampleBuffer::GetSampleCounts() const
{
    return this->sampleCounts;
}

} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif
