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

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "coreanimation/animresource.h"
#include "coreanimation/animsamplebuffer.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimJob : public Core::RefCounted
{
    DeclareClass(AnimJob);
public:
    /// constructor
    AnimJob();
    /// destructor
    virtual ~AnimJob();

    /// setup the anim job from an anim resource
    virtual void Setup(const Ptr<CoreAnimation::AnimResource>& animResource);
    /// discard the anim job object
    virtual void Discard();
    /// return true if between Setup/Discard
    bool IsValid() const;
    /// called when attached to anim sequencer
    virtual void OnAttachedToSequencer();
    /// called when removed from sequencer
    virtual void OnRemoveFromSequencer();
    /// return true if the job is currently attached to a sequencer
    bool IsAttachedToSequencer() const;

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
    /// set the track index (higher track numbers have a higher blend priority)
    void SetTrackIndex(IndexT trackIndex);
    /// get the track index of the anim job
    IndexT GetTrackIndex() const;
    
    /// set evaluation time
    void SetEvalTime(Timing::Tick time);
    /// get time of last evaluation
    Timing::Tick GetTime() const;
    /// evaluate the animation 
    virtual bool Evaluate();
    /// get result of the last evaluation
    const Ptr<CoreAnimation::AnimSampleBuffer>& GetResult() const;
    /// return true if the job has currently playing (EvalTime within start/end time)
    bool IsActive() const;
    /// return true if the job has been queued for playback but has not started yet
    bool IsPending() const;
    /// return true if the job has finished playback
    bool IsFinished() const;
    /// return true when the job has expired
    bool IsExpired() const;

private:
    Ptr<CoreAnimation::AnimResource> animResource;
    bool isAttachedToSequencer;
    IndexT trackIndex;
    Timing::Tick startTime;
    Timing::Tick duration;
    Timing::Tick fadeInTime;
    Timing::Tick expireTime;
    Timing::Tick curTime;
    Ptr<CoreAnimation::AnimSampleBuffer> sampleBuffer;
};

} // namespace Animation
//------------------------------------------------------------------------------
#endif
