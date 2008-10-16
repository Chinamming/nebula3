#pragma once
#ifndef ANIMATION_ANIMJOB_H
#define ANIMATION_ANIMJOB_H
//------------------------------------------------------------------------------
/**
    @class Animation::AnimJob
  
    Descibes a single animation sampling job in the AnimController. AnimJob
    objects have a start time and a duration and are arranged in parallel 
    tracks. The sampling results of parallel AnimJobs at a given point in
    time are mixed into a single resulting animation by the AnimController.
    Subclasses of AnimJob are used to implement specific tasks like
    a lookat-controller, IK, and so forth...

    FIXME: the current implementation of setting an absolute evaluation
    time doesn't allow to manipulate the playback speed (for this,
    advancing the time by a relative amount would be better).

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/weakptr.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsamplebuffer.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimSequencer;

class AnimJob : public Core::RefCounted
{
    __DeclareClass(AnimJob);
public:
    /// constructor
    AnimJob();
    /// destructor
    virtual ~AnimJob();

    /// called when attached to anim sequencer
    virtual void OnAttachedToSequencer(const Ptr<AnimSequencer>& animSequencer);
    /// called when removed from sequencer
    virtual void OnRemoveFromSequencer();
    /// return true if the job is currently attached to a sequencer
    bool IsAttachedToSequencer() const;

    /// evaluate the animation into the provided result buffer, returns current blend weight
    virtual float Evaluate(Timing::Tick time, const Ptr<CoreAnimation::AnimSampleBuffer>& result);
    /// return true if the job has currently playing (EvalTime within start/end time)
    bool IsActive(Timing::Tick time) const;
    /// return true if the job has been queued for playback but has not started yet
    bool IsPending(Timing::Tick time) const;
    /// return true if the job has finished playback
    bool IsFinished(Timing::Tick time) const;
    /// return true when the job has expired
    bool IsExpired(Timing::Tick time) const;

    /// set the start time of the anim job
    void SetStartTime(Timing::Tick time);
    /// get the start time of the anim job
    Timing::Tick GetStartTime() const;
    /// set the duration of the anim job (0 == infinite)
    void SetDuration(Timing::Tick time);
    /// get the duration of the anim job
    Timing::Tick GetDuration() const;
    /// return true if the anim job is infinite
    bool IsInfinite() const;
    /// set the fade-in time of the anim job
    void SetFadeInTime(Timing::Tick fadeInTime);
    /// get the fade-in time of the anim job
    Timing::Tick GetFadeInTime() const;
    /// set expire time, job will be removed from sequencer when expired
    void SetExpireTime(Timing::Tick expTime);
    /// get expire time
    Timing::Tick GetExpireTime() const;
    /// set blend weight of the anim job (default is 1.0)
    void SetBlendWeight(float w);
    /// get blend weight of the anim job
    float GetBlendWeight() const;
    /// set the blend priority (default priority is 0)
    void SetBlendPriority(int blendPriority);
    /// get the track index of the anim job
    int GetBlendPriority() const;
    
protected:
    /// compute current relative evaluation time, this is a helper method for subclasses
    Timing::Tick ComputeRelEvalTime(Timing::Tick time) const;
    /// compute current blend weight, this should take fade-in into account
    float ComputeBlendWeight(Timing::Tick relEvalTime) const;

    WeakPtr<AnimSequencer> animSequencer;
    int blendPriority;
    float blendWeight;
    Timing::Tick startTime;
    Timing::Tick duration;
    Timing::Tick fadeInTime;
    Timing::Tick expireTime;
};

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetStartTime(Timing::Tick t)
{
    this->startTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetStartTime() const
{
    return this->startTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetDuration(Timing::Tick t)
{
    this->duration = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetDuration() const
{
    return this->duration;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AnimJob::IsInfinite() const
{
    return (0 == this->duration);
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetFadeInTime(Timing::Tick t)
{
    this->fadeInTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetFadeInTime() const
{
    return this->fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetExpireTime(Timing::Tick t)
{
    this->expireTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Tick
AnimJob::GetExpireTime() const
{
    return this->expireTime;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetBlendWeight(float w)
{
    this->blendWeight = w;
}

//------------------------------------------------------------------------------
/**
*/
inline float
AnimJob::GetBlendWeight() const
{
    return this->blendWeight;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AnimJob::SetBlendPriority(int pri)
{
    this->blendPriority = pri;
}

//------------------------------------------------------------------------------
/**
*/
inline int
AnimJob::GetBlendPriority() const
{
    return this->blendPriority;
}

} // namespace Animation
//------------------------------------------------------------------------------
#endif
