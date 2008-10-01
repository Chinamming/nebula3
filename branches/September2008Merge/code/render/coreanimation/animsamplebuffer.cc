//------------------------------------------------------------------------------
//  animsamplebuffer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreanimation/animsamplebuffer.h"

namespace CoreAnimation
{
ImplementClass(CoreAnimation::AnimSampleBuffer, 'ASBF', Core::RefCounted);

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
AnimSampleBuffer::AnimSampleBuffer() :
    numSamples(0),
    sampleValues(0),
    sampleCounts(0)
{
    // empty
}    

//------------------------------------------------------------------------------
/**
*/
AnimSampleBuffer::~AnimSampleBuffer()
{
    if (this->IsValid())
    {
        this->Discard();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSampleBuffer::Setup(const Ptr<AnimResource>& animRes)
{
    n_assert(!this->IsValid());
    n_assert(0 == this->sampleValues);
    n_assert(0 == this->sampleCounts);

    this->animResource = animRes;
    this->numSamples = this->animResource->GetClipByIndex(0).GetNumCurves();
    this->sampleValues = (float4*) Memory::Alloc(Memory::ResourceHeap, this->numSamples * sizeof(float4));
    this->sampleCounts = (uchar*)  Memory::Alloc(Memory::ResourceHeap, this->numSamples * sizeof(uchar));
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSampleBuffer::Discard()
{
    n_assert(this->IsValid());
    n_assert(0 != this->sampleValues);
    n_assert(0 != this->sampleCounts);

    this->animResource = 0;
    Memory::Free(Memory::ResourceHeap, this->sampleValues);
    Memory::Free(Memory::ResourceHeap, this->sampleCounts);
    this->sampleValues = 0;
    this->sampleCounts = 0;
}

} // namespace CoreAnimation
