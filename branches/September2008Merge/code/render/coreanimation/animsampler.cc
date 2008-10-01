//------------------------------------------------------------------------------
//  animsampler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreanimation/animsampler.h"
#include "math/quaternion.h"

namespace CoreAnimation
{
using namespace Math;
using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
AnimSampler::AnimSampler() :
    numKeys(0),
    sampleBuffer(0),
    sampleCounts(0)
{
    // empty
}    

//------------------------------------------------------------------------------
/**
*/
AnimSampler::~AnimSampler()
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
AnimSampler::Setup(const Ptr<AnimResource>& res)
{
    n_assert(!this->IsValid());    
    n_assert(0 == this->sampleBuffer);
    n_assert(0 == this->sampleCounts);
    n_assert(res.isvalid());
    this->animResource = res;
    this->numKeys = this->animResource->GetClipByIndex(0).GetNumCurves();
    this->sampleBuffer = (float4*) Memory::Alloc(Memory::ResourceHeap, sizeof(float4) * this->numKeys);
    this->sampleCounts = (uchar*) Memory::Alloc(Memory::ResourceHeap, sizeof(uchar) * this->numKeys);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSampler::Discard()
{
    n_assert(this->IsValid());
    n_assert(0 != this->sampleBuffer);
    n_assert(0 != this->sampleCounts);
    this->animResource = 0;
    Memory::Free(Memory::ResourceHeap, this->sampleBuffer);
    Memory::Free(Memory::ResourceHeap, this->sampleCounts);
    this->sampleBuffer = 0;
    this->sampleCounts = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSampler::Sample(IndexT clipIndex, SampleType::Code sampleType, Tick time)
{
    n_assert(this->IsValid());
    n_assert((sampleType == SampleType::Step) || (sampleType == SampleType::Linear));
    
    const AnimClip& clip = this->animResource->GetClipByIndex(clipIndex);
    SizeT keyStride    = clip.GetKeyStride();
    Tick keyDuration   = clip.GetKeyDuration();
    Tick clipDuration  = clip.GetClipDuration();
    SizeT clipNumKeys  = clip.GetNumKeys();

    // clamp sample time to valid range as defined by pre-infinity and post-infinity type
    Tick sampleTime = time;
    if (sampleTime < 0)
    {
        if (clip.GetPreInfinityType() == InfinityType::Cycle)
        {
            sampleTime %= clipDuration;
        }
        else
        {
            sampleTime = 0;
        }
    }
    else if (sampleTime >= clipDuration)
    {
        if (clip.GetPostInfinityType() == InfinityType::Cycle)
        {
            sampleTime %= clipDuration;
        }
        else
        {
            sampleTime = clipDuration;
        }
    }

    // sampleTime is now >= 0 and <= clipDuration
    // compute the relative first key index, second key index and lerp value
    IndexT keyIndex0 = sampleTime / keyDuration;
    IndexT keyIndex1 = keyIndex0 + 1;
    keyIndex0 = n_iclamp(keyIndex0, 0, clipNumKeys);
    keyIndex1 = n_iclamp(keyIndex1, 0, clipNumKeys);
    Tick inbetweenTicks = sampleTime % keyDuration;
    float lerpValue = float(inbetweenTicks) / float(keyDuration);

    // sample curves...
    float4 f4Key0, f4Key1, f4SampleKey;
    quaternion qKey0, qKey1, qSampleKey;

    scalar* srcKeyStart = (scalar*) this->animResource->GetKeyBuffer()->Map();
    IndexT curveIndex;
    SizeT numCurves = clip.GetNumCurves();
    for (curveIndex = 0; curveIndex < numCurves; curveIndex++)
    {
        const AnimCurve& curve = clip.CurveByIndex(curveIndex);
        if (!curve.IsActive())
        {
            // curve is not active, reset sample count to 0
            this->sampleCounts[curveIndex] = 0;
        }
        else
        {
            // curve is active, set sample count to 1
            this->sampleCounts[curveIndex] = 1;
            scalar* resultPtr = (scalar*) &(this->sampleBuffer[curveIndex]);
            if (curve.IsStatic())
            {
                // the curve is constant, just copy the constant value into the result buffer
                curve.GetStaticKey().store(resultPtr);
            }
            else
            {
                // non-static curve, need to actually sample the result
                IndexT startKeyIndex = curve.GetFirstKeyIndex();

                // compute position of first key in source key buffer
                float* keyPtr0 = srcKeyStart + ((startKeyIndex + keyIndex0) * keyStride);
                if (SampleType::Step == sampleType)
                {
                    // if no interpolation needed, just copy the key to the sample buffer
                    f4SampleKey.load(keyPtr0);
                    f4SampleKey.store(resultPtr);
                }
                else
                {
                    // need to interpolate between 2 keys
                    float* keyPtr1 = srcKeyStart + ((startKeyIndex + keyIndex1) * keyStride);
                    if (curve.GetCurveType() == CurveType::Rotation)
                    {
                        // perform spherical interpolation
                        qKey0.load(keyPtr0);
                        qKey1.load(keyPtr1);
                        qSampleKey.slerp(qKey0, qKey1, lerpValue);
                        qSampleKey.store(resultPtr);
                    }
                    else
                    {
                        // perform linear interpolation
                        f4Key0.load(keyPtr0);
                        f4Key1.load(keyPtr1);
                        f4SampleKey.lerp(f4Key0, f4Key1, lerpValue);
                        f4SampleKey.store(resultPtr);
                    }
                }
            }
        }
    }
    // done
    this->animResource->GetKeyBuffer()->Unmap();
}

} // namespace CoreAnimation