#ifndef N_CHARACTER2_H
#define N_CHARACTER2_H
//------------------------------------------------------------------------------
/**
    @class nCharacter2
    @ingroup Character

    @brief Holds all the data necessary to animate an character in one place.

    (C) 2003 RadonLabs GmbH
*/
#include "core/refcounted.h"
#include "ncharskeleton.h"
#include "nebula2/anim/nanimclipscheduler.h"

namespace Models
{
    class CharacterNode;
}

namespace Nebula2
{
//------------------------------------------------------------------------------
struct nAnimation::Group::AnimHotspotInfo;

class nCharacter2 : public Core::RefCounted
{
    __DeclareClass(nCharacter2);
public:
    static const int animFramesPerSecond;
    
    // a blendmode defines which weighting information should be used
    enum BlendMode
    {
        Default,    // uses clip weights - for clips that animate the complete skeleton (also used when only one clip is active)
        Advanced    // uses curve weights - used, if one clip or multiple clips animate only selected joints of the skeleton
    };

    /// constructor
    nCharacter2();
    /// destructor
    virtual ~nCharacter2();
    /// set from another character
    void Set(const Ptr<nCharacter2>& other);
    /// get the embedded character skeleton
    nCharSkeleton& GetSkeleton();
    /// get the embedded clip scheduler
    const Ptr<nAnimClipScheduler>& GetClipScheduler() const;
    /// set pointer to an animation source which delivers the source data (not owned)
    void SetAnimation(const Ptr<nAnimation>& anim);
    /// get pointer to animation source (not owned)
    const Ptr<nAnimation>& GetAnimation() const;    
    /// evaluate the joint skeleton
    void EvaluateSkeleton(Timing::Time time, bool enforceEvaluation = false);
    /// enable/disable animation
    void SetAnimEnabled(bool b);
    /// get manual joint animation
    bool IsAnimEnabled() const;
    /// setups the clip scheduler
    void SetupClipScheduler(const Ptr<Models::CharacterNode>& charNode);
    /// debug feature: turn evaluation on/off for all characters
    static void SetGlobalAnimEnabled(bool b);
    /// debug feature: get global evaluation on/off state
    static bool IsGlobalAnimEnabled();
    /// get hotspots between to time stamps
    uint GetHotspotsInRange(Timing::Time fromTime, Timing::Time toTime, Util::Array<nAnimation::Group::AnimHotspotInfo>& outResult);
    /// set frame dirty flag, will evaluate skeleton next time the evaluate function is called
    void SetFrameDirty();

private:
    /// transfer the sampled animation values into the character skeleton
    void TransferSampledAnimToSkeleton();
    /// sample weighted values at a given time from nAnimation object
    bool Sample(Timing::Time time, Math::float4* keyArray, Math::float4* scratchKeyArray, int keyArraySize);
    /// performs weight blending - helper of nCharacter2::Sample()
    void PerformWeightedBlending(const nAnimation::Curve& curve, const Math::float4& sampleKey, Math::float4& mixedKey, int curveIndex, int firstAnimIndex, float clipWeight, float scaledWeight);
    /// calculate and return the weighting for a specific curve of a clip
    float CalculateCurveWeight(int clipIndex, int curveIndex) const;
    /// determine, if Advanced blend mode is required
    bool IsAdvancedBlendingRequired() const;

    enum
    {
        MaxJoints = 1024,
        MaxCurves = MaxJoints * 3,      ///< translate, rotate, scale per curve
    };

    nCharSkeleton charSkeleton;
    Ptr<nAnimClipScheduler> clipScheduler;
    Ptr<nAnimation> animation;

    static Math::float4 scratchKeyArray[MaxCurves];
    static Math::float4 keyArray[MaxCurves];
    static Math::float4 transitionKeyArray[MaxCurves];
    
    BlendMode blendMode;
    Util::FixedArray<float> weightAccums;    // 1 weightAccum per curve, required for correct blending in Advanced mode
    bool frameDirty;                        // flag to avoid mulitupdate per frame, have to be set to true from outside
    bool animEnabled;
    bool forceAdvancedBlending;          // might be slow, required for complex animation blending (lip animation etc.)
    bool firstEvaluation;

    static bool globalAnimEnabled;       
};

//------------------------------------------------------------------------------
/**
*/
inline nCharSkeleton&
nCharacter2::GetSkeleton()
{
    return this->charSkeleton;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<nAnimClipScheduler>& 
nCharacter2::GetClipScheduler() const
{
    return this->clipScheduler;
}

//------------------------------------------------------------------------------
/**
*/
inline void
nCharacter2::SetAnimation(const Ptr<nAnimation>& anim)
{
    n_assert(anim);
    this->animation = anim;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<nAnimation>&
nCharacter2::GetAnimation() const
{
    return this->animation;
}

//------------------------------------------------------------------------------
/**
*/
inline void
nCharacter2::SetAnimEnabled(bool b)
{
    this->animEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nCharacter2::IsAnimEnabled() const
{
    return (this->animEnabled && nCharacter2::globalAnimEnabled);
}

//------------------------------------------------------------------------------
/**
*/
inline void
nCharacter2::SetGlobalAnimEnabled(bool b)
{
    nCharacter2::globalAnimEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nCharacter2::IsGlobalAnimEnabled()
{
    return nCharacter2::globalAnimEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
nCharacter2::SetFrameDirty()
{
    this->frameDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
nCharacter2::Set(const Ptr<nCharacter2>& other)
{
    this->charSkeleton = other->charSkeleton;
    this->clipScheduler = other->clipScheduler;
    this->animation = other->animation;
    
    this->blendMode = other->blendMode;
    this->weightAccums = other->weightAccums;
    this->frameDirty = other->frameDirty;
    this->animEnabled = other->animEnabled;
    this->forceAdvancedBlending = other->forceAdvancedBlending;
    this->firstEvaluation = other->firstEvaluation;
}
}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
