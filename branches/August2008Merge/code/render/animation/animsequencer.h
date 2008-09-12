#pragma once
#ifndef ANIMATION_ANIMSEQEUNCER_H
#define ANIMATION_ANIMSEQUENCER_H
//------------------------------------------------------------------------------
/**
    @class Animation::AnimSequencer
  
    An AnimSequencer arranges AnimJobs along the time line in parallel
    tracks and produces a single, mixed samples result.

    (C) 2008 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "animation/animjob.h"

//------------------------------------------------------------------------------
namespace Animation
{
class AnimSequencer : public Core::RefCounted
{
    DeclareClass(AnimSequencer);
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

    /// add an anim job object
    void EnqueueAnimJob(const Ptr<AnimJob>& animJob);
    /// cancel a pending anim job, removing it from the sequencer
    void CancelAnimJob(const Ptr<AnimJob>& animJob);

    /// evaluate the sequencer at a given point in time
    void Evaluate(Timing::Time time);
    /// get the final sampled result of the last evaluation
    const Ptr<CoreAnimation::AnimSampleBuffer>& GetResult() const;

private:
    Ptr<CoreAnimation::AnimResource> animResource;
    Ptr<CoreAnimation::AnimSampleBuffer> sampleBuffer;
    Util::Array<Ptr<AnimJob> > animJobs; // currently sequenced anim jobs
};

} // namespace Animation
//------------------------------------------------------------------------------
#endif
