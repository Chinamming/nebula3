//------------------------------------------------------------------------------
//  playclipjob.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/playclipjob.h"
#include "animation/animsequencer.h"
#include "coreanimation/animsampler.h"

namespace Animation
{
__ImplementClass(Animation::PlayClipJob, 'PCLJ', Animation::AnimJob);

using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
PlayClipJob::PlayClipJob() :
    clipIndex(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
PlayClipJob::~PlayClipJob()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
float
PlayClipJob::Evaluate(Timing::Tick time,const Ptr<AnimSampleBuffer>& resultBuffer)
{
    n_assert(this->IsActive(time));
    n_assert(this->IsAttachedToSequencer());

    // compute relative evaluation time
    Timing::Tick relEvalTime = this->ComputeRelEvalTime(time);

    // perform sampling
    AnimSampler::Sample(this->animSequencer->GetAnimResource(), this->clipIndex, SampleType::Linear, relEvalTime, resultBuffer);

    // return the current blend weight back to the AnimSequencer
    float curBlendWeight = this->ComputeBlendWeight(relEvalTime);
    return curBlendWeight;
}

} // namespace Animation
