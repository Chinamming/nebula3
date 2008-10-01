//------------------------------------------------------------------------------
//  animmixer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreanimation/animmixer.h"
#include "math/quaternion.h"

namespace CoreAnimation
{
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
AnimMixer::AnimMixer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AnimMixer::~AnimMixer()
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
AnimMixer::Setup(const Ptr<AnimResource>& animRes)
{
    n_assert(!this->IsValid());
    n_assert(animRes.isvalid());
    n_assert(animRes->GetNumClips() > 0);
    this->animResource = animRes;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimMixer::Discard()
{
    n_assert(this->IsValid());
    this->animResource = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimMixer::Mix(const AnimSampler& src0, const AnimSampler& src1, float lerpValue, const AnimSampler& dst)
{
    n_assert(this->IsValid());

    // get a template clip from the anim resource, this defines the anim curve
    // types which we need for correct lerping
    const AnimClip& templClip = this->animResource->GetClipByIndex(0);
    n_assert(templClip.GetNumCurves() == src0.GetNumKeys());
    n_assert(templClip.GetNumCurves() == src1.GetNumKeys());
    n_assert(templClip.GetNumCurves() == dst.GetNumKeys());
    scalar* src0SamplePtr = (scalar*) src0.GetSamples();
    scalar* src1SamplePtr = (scalar*) src1.GetSamples();
    scalar* dstSamplePtr  = (scalar*) dst.GetSamples();
    uchar* src0SampleCounts = src0.GetSampleCounts();
    uchar* src1SampleCounts = src1.GetSampleCounts();
    uchar* dstSampleCounts = dst.GetSampleCounts();

    // perform the mixing...
    IndexT i;
    SizeT num = src0.GetNumKeys();
    quaternion q0, q1, qDst;
    float4 f0, f1, fDst;
    for (i = 0; i < num; i++)
    {
        const AnimCurve& curve = templClip.CurveByIndex(i);

        // read source sample counts and update destination sample counts
        uchar src0Count = *src0SampleCounts++;
        uchar src1Count = *src1SampleCounts++;
        
        // if lerp value is 0 or 1, set the left or right source sample counts 
        // to zero accordingly since they don't contribute to the result
        if (0.0f == lerpValue)
        {
            // only src0 contributes to the result
            src1Count = 0;
        }
        else if (1.0f == lerpValue)
        {
            // only src1 contributes to the result
            src0Count = 0;
        }
        *dstSampleCounts++ = src0Count + src1Count; 

        if ((src0Count > 0) && (src1Count > 0))
        {
            // both source keys are valid, need to mix into result
            if (curve.GetCurveType() == CurveType::Rotation)
            {
                // spherical interpolation
                q0.load((scalar*)src0SamplePtr);
                q1.load((scalar*)src1SamplePtr);
                qDst.slerp(q0, q1, lerpValue);
                qDst.store((scalar*)dstSamplePtr);
            }
            else
            {
                // linear interpolation
                f0.load((scalar*)src0SamplePtr);
                f1.load((scalar*)src1SamplePtr);
                fDst.lerp(f0, f1, lerpValue);
                fDst.store((scalar*)dstSamplePtr);
            }
        }
        else if (src0Count > 0)
        {
            // only src0 key is valid, copy to destination
            f0.load((scalar*)src0SamplePtr);
            f0.store((scalar*)dstSamplePtr);
        }
        else if (src1Count > 0)
        {
            // only src1 key is valid, copy to destination
            f1.load((scalar*)src1SamplePtr);
            f1.store((scalar*)dstSamplePtr);
        }
        else
        {
            // neither the src0 nor the src1 key contribute to the result,
            // the resulting destination sample count will be 0 and the
            // sample key is undefined
        }

        // update sample pointers
        src0SamplePtr++;
        src1SamplePtr++;
        dstSamplePtr++;
    }
}

} // namespace CoreAnimation
