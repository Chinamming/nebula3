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
void
AnimSampler::Sample(const Ptr<AnimResource>& animResource, 
                    IndexT clipIndex, 
                    SampleType::Code sampleType, 
                    Tick time, 
                    const Ptr<AnimSampleBuffer>& result)
{
    n_assert(animResource.isvalid());
    n_assert(result.isvalid());
    n_assert((sampleType == SampleType::Step) || (sampleType == SampleType::Linear));
    
    const AnimClip& clip = animResource->GetClipByIndex(clipIndex);
    SizeT keyStride    = clip.GetKeyStride();
    Tick keyDuration   = clip.GetKeyDuration();
    Tick clipDuration  = clip.GetClipDuration();
    SizeT clipNumKeys  = clip.GetNumKeys();
    n_assert(clip.GetNumCurves() == result->GetNumSamples());

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

    Math::float4* srcKeyStart = (Math::float4*) animResource->GetKeyBuffer()->Map();
    Math::float4* dstKeyBuffer = result->MapSamples();
    uchar* dstSampleCounts = result->MapSampleCounts();
    IndexT curveIndex;
    SizeT numCurves = clip.GetNumCurves();
    for (curveIndex = 0; curveIndex < numCurves; curveIndex++)
    {
        const AnimCurve& curve = clip.CurveByIndex(curveIndex);
        if (!curve.IsActive())
        {
            // curve is not active, set sample count to 0
            dstSampleCounts[curveIndex] = 0;
        }
        else
        {
            // curve is active, set sample count to 1
            dstSampleCounts[curveIndex] = 1;
            scalar* resultPtr = (scalar*) &(dstKeyBuffer[curveIndex]);
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
                scalar* keyPtr0 = (scalar*) (srcKeyStart + startKeyIndex + (keyIndex0 * keyStride));
                if (SampleType::Step == sampleType)
                {
                    // if no interpolation needed, just copy the key to the sample buffer
                    f4SampleKey.load(keyPtr0);
                    f4SampleKey.store(resultPtr);
                }
                else
                {
                    // need to interpolate between 2 keys
                    scalar* keyPtr1 = (scalar*) (srcKeyStart + startKeyIndex + (keyIndex1 * keyStride));
                    if (curve.GetCurveType() == CurveType::Rotation)
                    {
                        // perform spherical interpolation
                        qKey0.load(keyPtr0);
                        qKey1.load(keyPtr1);
                        qSampleKey = quaternion::slerp(qKey0, qKey1, lerpValue);
                        qSampleKey.store(resultPtr);
                    }
                    else
                    {
                        // perform linear interpolation
                        f4Key0.load(keyPtr0);
                        f4Key1.load(keyPtr1);
                        f4SampleKey = float4::lerp(f4Key0, f4Key1, lerpValue);
                        f4SampleKey.store(resultPtr);
                    }
                }
            }
        }
    }
    // done
    result->UnmapSampleCounts();
    result->UnmapSamples();
    animResource->GetKeyBuffer()->Unmap();
}

} // namespace CoreAnimation