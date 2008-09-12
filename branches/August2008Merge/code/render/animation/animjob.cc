//------------------------------------------------------------------------------
//  animjob.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animation/animjob.h"

namespace Animation
{
ImplementClass(Animation::AnimJob, 'AJOB', Core::RefCounted);

using namespace Util;
using namespace CoreAnimation;

//------------------------------------------------------------------------------
/**
*/
AnimJob::AnimJob() :
    isAttachedToSequencer(false),
    trackIndex(InvalidIndex),
    startTime(0),
    duration(0),
    fadeInTime(0),
    expireTime(0),
    curTime(0)
{
    // empty
}    

//------------------------------------------------------------------------------
/**
*/
AnimJob::~AnimJob()
{
    n_assert(!this->IsAttachedToSequencer());
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
AnimJob::Setup(const Ptr<AnimResource>& animRsrc)
{
    n_assert(!this->IsValid());
    n_assert(animRsrc.isvalid());
    n_assert(!this->isAttachedToSequencer);

    this->animResource = animRsrc;
    this->sampleBuffer = AnimSampleBuffer::Create();
    this->sampleBuffer->Setup(animRsrc);
}

//------------------------------------------------------------------------------
/**
*/
void
AnimJob::Discard()
{
    n_assert(this->IsValid());

    // IMPLEMENT ME!
}

} // namespace Animation
