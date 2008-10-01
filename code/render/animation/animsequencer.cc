//------------------------------------------------------------------------------
//  animsequencer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/animsequencer.h"

namespace Animation
{
ImplementClass(Animation::AnimSequencer, 'ASQR', Core::RefCounted);

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
    this->sampleBuffer = AnimSampleBuffer::Create();
    this->sampleBuffer->Setup(animRsrc);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSequencer::Discard()
{
    n_assert(this->IsValid());
    this->animResource = 0;
    this->sampleBuffer->Discard();
    this->sampleBuffer = 0;

    IndexT i;
    for (i = 0; i < this->animJobs.Size(); i++)
    {
        this->animJobs[i]->OnRemoveFromSequencer();
        this->animJobs[i] = 0;
    }
    this->animJobs.Clear();
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimSequencer::IsValid() const
{
    return this->animResource.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
void
AnimSequencer::EnqueueAnimJob(const Ptr<AnimJob>& animJob)
{
    n_assert(!animJob->IsAttachedToSequencer());
    this->animJobs.Append(animJob);
    animJob->OnAttachedToSequencer();
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
void
AnimSequencer::Evaluate(Timing::Time time)
{
    n_assert("IMPLEMENT ME!");
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<AnimSampleBuffer>&
AnimSequencer::GetResult() const
{
    return this->sampleBuffer;
}

} // namespace Animation