#pragma once
#ifndef ANIMATION_ANIMSEQEUNCER_H
#define ANIMATION_ANIMSEQUENCER_H
//------------------------------------------------------------------------------
/**
    @class Animation::AnimSequencer
  
    An AnimSequencer object arranges AnimJobs along the time line to
    produce a single, priority-blended result. AnimJobs which are overlapping
    on the time-line will be blended by the following rules:

    - AnimJobs with a higher blend priority dominate lower-priority anim jobs
    - if AnimJobs have the same blend priority, the start time of the anim
      job is used to determine blend priority (jobs which start later dominate
      jobs which start earlier)

    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "animation/animjob.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimSequencer : public Core::RefCounted
{
    __DeclareClass(AnimSequencer);
public:
    /// constructor
    AnimSequencer();
    /// destructor
    virtual ~AnimSequencer();

    /// setup the animation controller
    void Setup(const Ptr<CoreAnimation::AnimResource>& animResource);
    /// discard the anim sequencer
    void Discard();
    /// return true if between Setup/Discard
    bool IsValid() const;

    /// add an anim job object the sequencer
    void AddAnimJob(const Ptr<AnimJob>& animJob);
    /// cancel a pending anim job, removing it from the sequencer
    void CancelAnimJob(const Ptr<AnimJob>& animJob);

    /// evaluate the sequencer at a given point in time
    bool Evaluate(Timing::Tick time);
    /// get the final sampled result of the last evaluation
    const Ptr<CoreAnimation::AnimSampleBuffer>& GetResult() const;
    /// get pointer to animation resource object
    const Ptr<CoreAnimation::AnimResource>& GetAnimResource() const;

private:
    /// delete expired anim jobs
    void RemoveExpiredAnimJobs(Timing::Tick time);

    static const int NumSampleBuffers = 2;

    Ptr<CoreAnimation::AnimResource> animResource;
    Ptr<CoreAnimation::AnimSampleBuffer> srcSampleBuffer[NumSampleBuffers];
    Ptr<CoreAnimation::AnimSampleBuffer> dstSampleBuffer;
    Util::Array<Ptr<AnimJob> > animJobs; // currently sequenced anim jobs
};

//------------------------------------------------------------------------------
/**
*/
inline bool
AnimSequencer::IsValid() const
{
    return this->animResource.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreAnimation::AnimSampleBuffer>&
AnimSequencer::GetResult() const
{
    return this->dstSampleBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreAnimation::AnimResource>&
AnimSequencer::GetAnimResource() const
{
    return this->animResource;
}

} // namespace Animation
//------------------------------------------------------------------------------
#endif
