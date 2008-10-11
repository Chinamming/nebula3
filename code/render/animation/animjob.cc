//------------------------------------------------------------------------------
//  animjob.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/animjob.h"
#include "animation/animsequencer.h"

namespace Animation
{
__ImplementClass(Animation::AnimJob, 'AJOB', Core::RefCounted);

using namespace Util;
using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
AnimJob::AnimJob() :
    blendPriority(0),
    blendWeight(1.0f),
    startTime(0),
    duration(0),
    fadeInTime(0),
    expireTime(1000)
{
    // empty
}    

//------------------------------------------------------------------------------
/**
*/
AnimJob::~AnimJob()
{
    n_assert(!this->IsAttachedToSequencer());
}

//------------------------------------------------------------------------------
/**
*/
void
AnimJob::OnAttachedToSequencer(const Ptr<AnimSequencer>& animSeq)
{
    n_assert(!this->IsAttachedToSequencer());
    n_assert(animSeq.isvalid());
    this->animSequencer = animSeq;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimJob::OnRemoveFromSequencer()
{
    n_assert(this->IsAttachedToSequencer());
    this->animSequencer = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimJob::IsAttachedToSequencer() const
{
    return this->animSequencer.isvalid();
}

//------------------------------------------------------------------------------
/**
    This method will return true if the current eval time is between
    the start time and end time of the anim job.
*/
bool
AnimJob::IsActive(Timing::Tick time) const
{
    if (this->IsInfinite())
    {
        return (time >= this->startTime);
    }
    else
    {
        return (time >= this->startTime) && (time < (this->startTime + this->duration));
    }
}

//------------------------------------------------------------------------------
/**
    This method will return true as long as the current eval time is
    before the start time (the job hasn't started yet).
*/
bool
AnimJob::IsPending(Timing::Tick time) const
{
    return (time < this->startTime);
}

//------------------------------------------------------------------------------
/**
    This method will return true if the current eval time is greater
    then the end time of the job.
*/
bool
AnimJob::IsFinished(Timing::Tick time) const
{
    if (this->IsInfinite())
    {
        return false;
    }
    else
    {
        return (time >= (this->startTime + this->duration));
    }
}

//------------------------------------------------------------------------------
/**
    This method will return true if the current eval time is greater
    then the end time plus the expiration time. This means the job
    can be removed from the anim sequencer.
*/
bool
AnimJob::IsExpired(Timing::Tick time) const
{
    if (this->IsInfinite())
    {
        return false;
    } 
    else
    {
        return (time >= (this->startTime + this->duration + this->expireTime));
    }
}

//------------------------------------------------------------------------------
/**
    Compute the relative evaluation time, this method is called by subclasses
    to get the clip-relative sample time. This method is allowed to return
    a negative time value, but since usually only active anim jobs are called
    back by the anim sequencer this will normally not happen.

    FIXME: this method must be more advanced if playback speed should be
    be implemented.
*/
Timing::Tick
AnimJob::ComputeRelEvalTime(Timing::Tick time) const
{
    return (time - this->startTime);
}

//------------------------------------------------------------------------------
/**
    This is a helper method for subclasses and returns the current blend
    weight for the current relative evaluation time, taking the fade-in
    phase into account.
*/
float
AnimJob::ComputeBlendWeight(Timing::Tick relEvalTime) const
{
    n_assert(relEvalTime >= 0);
    if (relEvalTime >= this->fadeInTime)
    {
        // we're past the fade-in phase, return the actual blend weight
        return this->blendWeight;
    }
    else
    {
        // we're withing the fade-in phase, return an interpolated blend weight
        n_assert(this->fadeInTime > 0);
        float curWeight = this->blendWeight * (float(relEvalTime) / float(fadeInTime));
        return curWeight;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by the AnimSequencer when this job is active
    (the current eval time is between the start and end time of the job).
    The AnimJob object is expected to fill the provided AnimSampleBuffer 
    with a result (sampled keys and sample counts, the sample counts indicate 
    whether a given sample contributes to the final blended result. The method
    is expected to return the blend weight for mixing by the AnimSequencer
    (usually the job's blend weight is returned, unless in the fade-in-phase
    where an interpolated blend weight from 0 to max should be returned,
    however, derived anim jobs are basically to return whatever blend
    weight they want).

    This method is usually implemented by subclasses.
*/
float
AnimJob::Evaluate(Timing::Tick time, const Ptr<AnimSampleBuffer>& result)
{
    // implement in subclass!
    return 0.0f;
}

} // namespace Animation
