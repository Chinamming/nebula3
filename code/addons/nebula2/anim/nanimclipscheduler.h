#ifndef N_ANIMCLIPSCHEDULER_H
#define N_ANIMCLIPSCHEDULER_H
//------------------------------------------------------------------------------
/**
    @class nAnimClipScheduler
    @ingroup Anim2

    (C) 2007 RadonLabs GmbH

    @brief An animation clip scheduler contains nAnimClip related data: clip weights,
    fade times, run times, start times, durations etc.
    The scheduler controls clip fading and provides clip data for nCharacter2's skeleton
    evaluation and clip sampling.
    Only animation clips with a weight value greater 0.0f are active and used for 
    blending calculations.
*/
#include "core/refcounted.h"
#include "nanimclip.h"

namespace Nebula2
{
//------------------------------------------------------------------------------
class nAnimClipScheduler : public Core::RefCounted
{
    __DeclareClass(nAnimClipScheduler);
public:
    enum FadeType
    {
        NoFade,
        FadeIn,
        FadeOut,
    };

    enum InfluenceType
    {
        CompleteSkeleton,
        SelectedJoints
    };

    /// constructor
    nAnimClipScheduler();
    /// destructor
    ~nAnimClipScheduler();

    /// begin defining animation clips
    void BeginClips(int num);
    /// set an animation clip
    void SetClip(IndexT clipIndex, Util::String clipName, float weight, float runTime, float duration, float fadeOutTime, float fadeInTime, int animGroupIndex, nAnimation::Group::LoopType loopType, InfluenceType influence);
    /// finish defining animation clips
    void EndClips();
    
    /// sets a new fade state and other fade-related data on a clip
    void FadeClipAt(IndexT clipIndex, Timing::Time currentTime, FadeType newFadeState, float targetWeight = 0.0f);
    
    /// returns the scheduler's current dirty state
    bool IsDirty() const;
    /// is valid?
    bool IsValid() const;
    /// is an active clip at the given index - weight > 0.0f
    bool IsClipActiveAt(IndexT clipIndex) const;

    /// GET only
    /// get number of animation clips
    int GetNumClips() const;
    /// returns the number of currently active clips
    int GetNumActiveClips() const;
    /// returns the sum of all clip weights
    float GetWeightSum() const;
    /// get the clip time (time since clip start) of a clip, the returned clip time for clips with loop type CLAMP is clamped
    Timing::Time GetClipTimeAt(IndexT clipIndex, Timing::Time currentTime, bool calcForSampleTimeClips = false, bool useTimeFactor = true) const;
    /// get the name of a clip
    Util::String GetClipNameAt(IndexT clipIndex) const;
    /// get the loop type of a clip
    nAnimation::Group::LoopType GetClipLoopTypeAt(IndexT clipIndex) const;
    /// get the duration of a clip
    Timing::Time GetClipDurationAt(IndexT clipIndex) const;
    /// get the animation group index of a clip
    int GetClipAnimGroupIndexAt(IndexT clipIndex) const;
    /// get an array of indices of currently active clips
    const Util::Array<int>& GetActiveClipIndices() const;
    /// get the influence type of a clip
    InfluenceType GetClipInfluenceTypeAt(IndexT clipIndex) const;
    /// get the current fade state of a clip
    FadeType GetClipFadeTypeAt(IndexT clipIndex) const;

    /// GET and SET
    /// set the fade out time for a clip
    void SetClipFadeOutTimeAt(IndexT clipIndex, float time);
    /// get the fade out time of a clip
    Timing::Time GetClipFadeOutTimeAt(IndexT clipIndex) const;
    /// set the fade in time for a clip
    void SetClipFadeInTimeAt(IndexT clipIndex, float time);
    /// get the fade in time of a clip
    Timing::Time GetClipFadeInTimeAt(IndexT clipIndex) const;
    /// set the weight for a clip
    void SetClipWeightAt(IndexT clipIndex, float weight);    
    /// get the weight of a clip
    float GetClipWeightAt(IndexT clipIndex) const;    
    /// get the clip run time (= number of runs: 0.0 -> loop, 2.2 -> 2.2 runs)
    Timing::Time GetClipRunTimeAt(IndexT clipIndex) const;
    /// set the clip run time (= number of runs: 0.0 -> loop, 2.2 -> 2.2 runs)
    void SetClipRunTimeAt(IndexT clipIndex, float runTime);
    /// resets clip start time
    void SetClipStartTimeAt(IndexT clipIndex, float time);
    /// get the start time of a clip
    Timing::Time GetClipStartTimeAt(IndexT clipIndex) const;    
    /// set the sample time for a clip
    void SetClipSampleTimeAt(IndexT clipIndex, float sampleTime);    
    /// get the sample time of a clip
    Timing::Time GetClipSampleTimeAt(IndexT clipIndex) const;
    /// set the time factor for a clip
    void SetClipTimeFactorAt(IndexT clipIndex, float timeFactor);    
    /// get the time factor of a clip
    float GetClipTimeFactorAt(IndexT clipIndex) const;
    /// get the clip start offset
    Timing::Time GetClipTimeOffsetAt(IndexT clipIndex) const;
    /// set the clip start offset
    void SetClipTimeOffsetAt(IndexT clipIndex, float t);

private:
    friend class nCharacter2;

    /// updates clips and their data
    void Update(Timing::Time time);
    /// does the fade in/out of clips - calculates new weights
    void UpdateClipWeight(IndexT clipIndex, Timing::Time time, FadeType fadeState);
    
    struct nAnimClipData
    {
        Util::String clipName;
        float currentWeight;
        Timing::Time desiredRunTime;
        Timing::Time duration;
        Timing::Time fadeOutTime;
        Timing::Time fadeInTime;
        Timing::Time fadeStartTime;
        float fadeStartWeight;
        float fadeTargetWeight;
        Timing::Time sampleTime;
        Timing::Time startTime;
        float timeFactor;
        Timing::Time timeOffset;
        int animGroupIndex;
        nAnimation::Group::LoopType loopType;
        FadeType fadeState;
        InfluenceType influenceType;
    };

    Util::FixedArray<nAnimClipData> clipData; // data for all clips
    Util::Array<int> activeClipIndices;
    
    int numActiveClips;
    bool isDirtyThisFrame;
    bool isDirty;           // dirty scheduler -> curve weights during Advanced blending must be updated in nCharacter2
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
nAnimClipScheduler::IsDirty() const
{
    return this->isDirtyThisFrame;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nAnimClipScheduler::SetClipFadeOutTimeAt(IndexT clipIndex, float time)
{
    this->clipData[clipIndex].fadeOutTime = time;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
nAnimClipScheduler::GetClipFadeOutTimeAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].fadeOutTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nAnimClipScheduler::SetClipFadeInTimeAt(IndexT clipIndex, float time)
{
    this->clipData[clipIndex].fadeInTime = time;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
nAnimClipScheduler::GetClipFadeInTimeAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nAnimClipScheduler::IsValid() const
{
    return (0 != this->clipData.Size());
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nAnimClipScheduler::IsClipActiveAt(IndexT clipIndex) const
{
    return ((this->clipData[clipIndex].currentWeight > 0.0f) || FadeIn == this->clipData[clipIndex].fadeState);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nAnimClipScheduler::BeginClips(int num)
{
    this->clipData.SetSize(num);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nAnimClipScheduler::EndClips()
{
    // empty
}


//------------------------------------------------------------------------------
/**
*/
inline
int
nAnimClipScheduler::GetNumClips() const
{
    return this->clipData.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline 
float
nAnimClipScheduler::GetClipWeightAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].currentWeight;
}

//------------------------------------------------------------------------------
/**
*/
inline 
Timing::Time
nAnimClipScheduler::GetClipStartTimeAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].startTime;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
nAnimClipScheduler::SetClipWeightAt(IndexT clipIndex, float weight)
{  
    this->clipData[clipIndex].currentWeight = weight;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nAnimClipScheduler::GetNumActiveClips() const
{
    return this->numActiveClips;
}

//------------------------------------------------------------------------------
/**
*/
inline 
Timing::Time
nAnimClipScheduler::GetClipRunTimeAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].desiredRunTime;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
nAnimClipScheduler::SetClipRunTimeAt(IndexT clipIndex, float runTime)
{
    this->clipData[clipIndex].desiredRunTime = runTime;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline 
Util::String
nAnimClipScheduler::GetClipNameAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].clipName;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
nAnimClipScheduler::SetClipStartTimeAt(IndexT clipIndex, float time)
{
    this->clipData[clipIndex].startTime = time;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
nAnimation::Group::LoopType 
nAnimClipScheduler::GetClipLoopTypeAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].loopType;
}

//------------------------------------------------------------------------------
/**
*/
inline 
Timing::Time
nAnimClipScheduler::GetClipSampleTimeAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].sampleTime;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
nAnimClipScheduler::SetClipSampleTimeAt(IndexT clipIndex, float sampleTime)
{
    this->clipData[clipIndex].sampleTime = sampleTime;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline 
float
nAnimClipScheduler::GetClipTimeFactorAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].timeFactor;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
nAnimClipScheduler::SetClipTimeFactorAt(IndexT clipIndex, float timeFactor)
{
    this->clipData[clipIndex].timeFactor = timeFactor;
    this->isDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline 
Timing::Time
nAnimClipScheduler::GetClipDurationAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].duration;
}

//------------------------------------------------------------------------------
/**
*/
inline 
int
nAnimClipScheduler::GetClipAnimGroupIndexAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].animGroupIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline 
const Util::Array<int>&
nAnimClipScheduler::GetActiveClipIndices() const
{
    return this->activeClipIndices;
}

//------------------------------------------------------------------------------
/**
*/
inline
nAnimClipScheduler::InfluenceType
nAnimClipScheduler::GetClipInfluenceTypeAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].influenceType;
}

//------------------------------------------------------------------------------
/**
*/
inline
nAnimClipScheduler::FadeType
nAnimClipScheduler::GetClipFadeTypeAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].fadeState;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
nAnimClipScheduler::GetClipTimeOffsetAt(IndexT clipIndex) const
{
    return this->clipData[clipIndex].timeOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nAnimClipScheduler::SetClipTimeOffsetAt(IndexT clipIndex, float timeOffset)
{
    this->clipData[clipIndex].timeOffset = timeOffset;
}
}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
