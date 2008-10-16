//------------------------------------------------------------------------------
//  animsequencer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/animsequencer.h"
#include "coreanimation/animmixer.h"

namespace Animation
{
__ImplementClass(Animation::AnimSequencer, 'ASQR', Core::RefCounted);

using namespace Util;
using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
AnimSequencer::AnimSequencer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AnimSequencer::~AnimSequencer()
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
AnimSequencer::Setup(const Ptr<AnimResource>& animRsrc)
{
    n_assert(!this->IsValid());
    n_assert(animRsrc.isvalid());
    
    this->animResource = animRsrc;
    IndexT i;
    for (i = 0; i < NumSampleBuffers; i++)
    {
        this->srcSampleBuffer[i] = AnimSampleBuffer::Create();
        this->srcSampleBuffer[i]->Setup(animRsrc);
    }
    this->dstSampleBuffer = AnimSampleBuffer::Create();
    this->dstSampleBuffer->Setup(animRsrc);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSequencer::Discard()
{
    n_assert(this->IsValid());
    this->animResource = 0;
    IndexT i;
    for (i = 0; i < NumSampleBuffers; i++)
    {
        this->srcSampleBuffer[i]->Discard();
        this->srcSampleBuffer[i] = 0;
    }
    this->dstSampleBuffer->Discard();
    this->dstSampleBuffer = 0;
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        this->animJobs[i]->OnRemoveFromSequencer();
        this->animJobs[i] = 0;
    }
    this->animJobs.Clear();
}

//------------------------------------------------------------------------------
/**
    Priority-inserts an anim job. Higher priorities are inserted at the 
    end of the anim job array. Within the same priority value, anim jobs
    are sorted by start time.
*/
void
AnimSequencer::AddAnimJob(const Ptr<AnimJob>& animJob)
{
    n_assert(!animJob->IsAttachedToSequencer());
    
    // find the right insertion index
    IndexT i = 0;
    while ((i < this->animJobs.Size()) && 
           (this->animJobs[i]->GetBlendPriority() <= animJob->GetBlendPriority()))
    {
        if (this->animJobs[i]->GetBlendPriority() == animJob->GetBlendPriority())
        {
            if (this->animJobs[i]->GetStartTime() > animJob->GetStartTime())
            {
                break;
            }
        }
        i++;
    }
    this->animJobs.Insert(i, animJob);
    
    // notify anim job that it has been attached to a sequencer    
    animJob->OnAttachedToSequencer(this);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSequencer::CancelAnimJob(const Ptr<AnimJob>& animJob)
{
    n_assert(animJob->IsAttachedToSequencer());
    IndexT index = this->animJobs.FindIndex(animJob);
    n_assert(InvalidIndex != index);
    animJob->OnRemoveFromSequencer();
    this->animJobs.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimSequencer::Evaluate(Timing::Tick time)
{
    // first discard any expired anim jobs
    this->RemoveExpiredAnimJobs(time);

    // get the number of active anim jobs beforehand, if only one active anim job
    // exists, sample directly into the destination buffer without mixing
    IndexT i;
    IndexT numActiveAnimJobs = 0;
    IndexT activeAnimJobIndex = InvalidIndex;
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        if (this->animJobs[i]->IsActive(time))
        {
            numActiveAnimJobs++;
            activeAnimJobIndex = i;
        }
    }

    // if no active anim job exists, return false
    if (0 == numActiveAnimJobs)
    {
        return false;
    }
    else if (1 == numActiveAnimJobs)
    {
        // if only one anim job currently active, no mixing is needed, sample
        // directly into the destination buffer
        this->animJobs[activeAnimJobIndex]->Evaluate(time, this->dstSampleBuffer);
    }
    else
    {
        // more then one active anim job at the current time stamp, need to perform mixing
        activeAnimJobIndex = 0;
        for (i = 0; i < this->animJobs.Size(); i++)
        {
            if (this->animJobs[i]->IsActive(time))
            {
                IndexT srcIndex = activeAnimJobIndex++ & 1;

                // sample current anim job
                float curWeight = this->animJobs[i]->Evaluate(time, this->srcSampleBuffer[srcIndex]);

                // blend with previous anim buffer into destination sample buffer
                if (i > 0)
                {
                    AnimMixer::Mix(this->animResource, 
                                   this->srcSampleBuffer[1 - srcIndex], 
                                   this->srcSampleBuffer[srcIndex],
                                   curWeight,
                                   this->dstSampleBuffer);
                }
            }
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Iterates through the currently attached anim jobs and
    removes all anim jobs which are expired. This method is called from
    Evaluate().
*/
void
AnimSequencer::RemoveExpiredAnimJobs(Timing::Tick time)
{
    IndexT i;
    for (i = this->animJobs.Size() - 1; i != InvalidIndex; i--)
    {
        if (this->animJobs[i]->IsExpired(time))
        {
            this->animJobs[i]->OnRemoveFromSequencer();
            this->animJobs.EraseIndex(i);
        }
    }
}   

} // namespace Animation