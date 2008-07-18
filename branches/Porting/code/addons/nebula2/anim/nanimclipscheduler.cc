//------------------------------------------------------------------------------
//  nanimclipscheduler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nanimclipscheduler.h"

namespace Nebula2
{
ImplementClass(nAnimClipScheduler, 'CLSH', Core::RefCounted);
//------------------------------------------------------------------------------
/**
*/
nAnimClipScheduler::nAnimClipScheduler() :
    numActiveClips(0),
    isDirtyThisFrame(true),
    isDirty(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
nAnimClipScheduler::~nAnimClipScheduler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
nAnimClipScheduler::SetClip(IndexT index, Util::String clipName, float weight, float runTime, float duration, 
    float fadeOutTime, float fadeInTime, int animGroupIndex, nAnimation::Group::LoopType loopType,
    InfluenceType influence)
{
    n_assert(index >= 0);
    n_assert(weight >= 0.0f && weight <= 1.0f);

    this->clipData[index].clipName = clipName;
    this->clipData[index].currentWeight = weight;
    this->clipData[index].desiredRunTime = runTime;
    this->clipData[index].duration = duration;
    this->clipData[index].fadeStartTime = 0.0f;
    this->clipData[index].fadeStartWeight = 0.0f;
    this->clipData[index].fadeTargetWeight = 1.0f;
    this->clipData[index].fadeOutTime = fadeOutTime;
    this->clipData[index].fadeInTime = fadeInTime;
    this->clipData[index].startTime = 0.0f;
    this->clipData[index].loopType = loopType;
    this->clipData[index].fadeState = NoFade;
    this->clipData[index].sampleTime = 0.0f;
    this->clipData[index].timeFactor = 1.0f;
    this->clipData[index].timeOffset = 0.0f;
    this->clipData[index].animGroupIndex = animGroupIndex;
    this->clipData[index].influenceType = influence;
}

//------------------------------------------------------------------------------
/**
    Update clips and their data
*/
void
nAnimClipScheduler::Update(Timing::Time time)
{
    // FIXME: this isn't very useful if Update() is called more then 2x per frame!?
    this->isDirtyThisFrame = this->isDirty;
    this->isDirty = false;
    this->numActiveClips = 0;
    this->activeClipIndices.Clear();
    FadeType fadeState;

    IndexT clipIndex;
    for(clipIndex = 0; clipIndex < this->clipData.Size(); clipIndex++)
    {       
        fadeState = this->clipData[clipIndex].fadeState;
        
        if(this->IsClipActiveAt(clipIndex) || FadeIn == fadeState)
        {
            const Timing::Time desiredRunTime = this->clipData[clipIndex].desiredRunTime;
            
            // run time check only for clips that do not loop
            if(desiredRunTime > 0.0f)
            {
                const Timing::Time duration = this->clipData[clipIndex].duration / this->clipData[clipIndex].timeFactor;
                const Timing::Time fadeOutTime = n_min(duration, this->clipData[clipIndex].fadeOutTime);
                const Timing::Time currentRunTime = (this->GetClipTimeAt(clipIndex, time, true, false) / duration) + (fadeOutTime / duration);
                                
                if(FadeOut != fadeState && currentRunTime >= desiredRunTime)
                {
                    this->FadeClipAt(clipIndex, time, FadeOut);
                    fadeState = FadeOut;
                    this->isDirty = true;
                }
            }
            
            // don't handle NoFade clips
            if(NoFade != fadeState)
            {
                this->UpdateClipWeight(clipIndex, time, fadeState);
            }

            // count only active clips - weight could be 0.0 due to fade out
            if(this->IsClipActiveAt(clipIndex))
            {
                this->numActiveClips++;
                this->activeClipIndices.Append(clipIndex);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
float
nAnimClipScheduler::GetWeightSum() const
{
    float sum = 0.0f;

    IndexT clipIndex;
    for (clipIndex = 0; clipIndex < this->activeClipIndices.Size(); clipIndex++)
    {
        sum += this->clipData[this->activeClipIndices[clipIndex]].currentWeight;
    }

    return sum;
}

//------------------------------------------------------------------------------
/**
    Sets the fade flag and other fade-related data for a clip
*/
void
nAnimClipScheduler::FadeClipAt(IndexT clipIndex, Timing::Time currentTime, FadeType newFadeState, float targetWeight)
{
    n_assert(clipIndex >= 0 && clipIndex < (IndexT)this->GetNumClips());

    // set fade data
    nAnimClipData& animClipData = this->clipData[clipIndex];
    animClipData.fadeStartTime = currentTime;
    animClipData.fadeStartWeight = this->clipData[clipIndex].currentWeight;
    animClipData.fadeTargetWeight = targetWeight;
    animClipData.fadeState = newFadeState;
}

//------------------------------------------------------------------------------
/**
    Controls the fading of clips - calculates new weights
*/
void 
nAnimClipScheduler::UpdateClipWeight(IndexT clipIndex, Timing::Time time, FadeType fadeState)
{
    float newWeight, startWeight, deltaWeight, targetWeight;
    Timing::Time startTime, deltaTime, currentDeltaTime;

    // init times
    startTime = this->clipData[clipIndex].fadeStartTime;
    currentDeltaTime = time - startTime;
    
    // set fade time depending on fade type
    if(FadeOut == fadeState)
    {
        deltaTime = this->clipData[clipIndex].fadeOutTime;
    }
    else
    {
        deltaTime = this->clipData[clipIndex].fadeInTime;
    }
    
    // fading nearly complete? then finish...
    if(deltaTime - currentDeltaTime < 0.02f || currentDeltaTime >= this->clipData[clipIndex].duration / this->clipData[clipIndex].timeFactor)
    {
        this->clipData[clipIndex].currentWeight = this->clipData[clipIndex].fadeTargetWeight;
        this->clipData[clipIndex].fadeState = NoFade;
        this->isDirty = true;
        return;
    }

    // skip calculations, if fading started this frame
    if(currentDeltaTime > 0.0f)
    {
        startWeight = this->clipData[clipIndex].fadeStartWeight;
        targetWeight = this->clipData[clipIndex].fadeTargetWeight;
        deltaWeight = targetWeight - startWeight;
        
        // calculate the new weight
        newWeight = startWeight + (deltaWeight / (float)deltaTime) * (float)currentDeltaTime;

        this->clipData[clipIndex].currentWeight = newWeight;
        this->isDirty = true;
    }
}

//------------------------------------------------------------------------------
/**
    Get the clip time (time since clip start) of a clip.
    Special cases:
    - the returned clip time for clips with loop type CLAMP is clamped
    - the clip's sample time is returned, if it's set to a value greater than 0.0f
      and calcForSampleTimeClips is 'false'

    @param  calcForSampleTimeClips  (default -> false) enable this, if you need 
            the 'correct' clip time of a sample time clip - only used by 
            nAnimClipScheduler::Update() to identify a sample time clip that has 
            exceeded its runTime
*/
Timing::Time 
nAnimClipScheduler::GetClipTimeAt(IndexT clipIndex, Timing::Time currentTime, bool calcForSampleTimeClips, bool useTimeFactor) const
{
    Timing::Time clipTime;
    
    // sample time: clip will be sampled only at the given time stamp
    // Note: clamp clips cannot be sampled at a time stamp > clip duration
    const nAnimClipData& animClipData = this->clipData[clipIndex];
    if (animClipData.sampleTime > 0.0f && !calcForSampleTimeClips)
    {
        clipTime = animClipData.sampleTime;        
    }
    // clip time: (current time - clip start time) * clip time factor
    else
    {
        clipTime = currentTime - animClipData.startTime;

        if(useTimeFactor)
        {
            clipTime *= animClipData.timeFactor;    
        }
    }

    // clamp clip time for clips with looptype clamp
    if ((animClipData.loopType == nAnimation::Group::Clamp) && (clipTime >= animClipData.duration / animClipData.timeFactor))
    {
        clipTime = this->clipData[clipIndex].duration / animClipData.timeFactor;
    }
    return clipTime;
}
}; // namespace Nebula2