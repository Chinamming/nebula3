#pragma once
#ifndef COREANIMATION_ANIMSAMPLEBUFFER_H
#define COREANIMATION_ANIMSAMPLEBUFFER_H
//------------------------------------------------------------------------------
/**
    @class CoreAnimation::AnimSampleBuffer
  
    Stores the result of an animation sampling operation, stores 
    samples key values and sample-counts which keep track of the number 
    of samples which contributed to a mixing result (this is necessary
    for correct mixing of partial animations).
     
    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "coreanimation/animresource.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class AnimSampleBuffer : public Core::RefCounted
{
    __DeclareClass(AnimSampleBuffer);
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
    /// gain read/write access to sample buffer
    Math::float4* MapSamples();
    /// give up access to sample buffer
    void UnmapSamples();
    /// gain read/write access to sample counts
    uchar* MapSampleCounts();
    /// give up access to sample counts
    void UnmapSampleCounts();

private:
    Ptr<AnimResource> animResource;
    SizeT numSamples;
    Math::float4* samples;
    uchar* sampleCounts;
    bool samplesMapped;
    bool sampleCountsMapped;
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

} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif
