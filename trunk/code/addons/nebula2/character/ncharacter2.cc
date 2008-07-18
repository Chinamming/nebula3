//------------------------------------------------------------------------------
//  ncharacter2.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ncharacter2.h"
#include "models/nodes/characternode.h"

namespace Nebula2
{
ImplementClass(nCharacter2, 'CHA2', Core::RefCounted);

Math::float4 nCharacter2::scratchKeyArray[MaxCurves];
Math::float4 nCharacter2::keyArray[MaxCurves];
Math::float4 nCharacter2::transitionKeyArray[MaxCurves];
const int nCharacter2::animFramesPerSecond = 25;
bool nCharacter2::globalAnimEnabled = true;

using namespace Math;
//------------------------------------------------------------------------------
/**
*/
nCharacter2::nCharacter2() :
    animEnabled(true),
    firstEvaluation(true),
    forceAdvancedBlending(false),    
    blendMode(Default),
    frameDirty(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
nCharacter2::~nCharacter2()
{
}

//------------------------------------------------------------------------------
/**
*/
void
nCharacter2::EvaluateSkeleton(Timing::Time time, bool enforceEvaluation)
{      
    // only evaluate if anim is enabled and evaluation is enforced or there was no evaluation this frame yet
    if (this->IsAnimEnabled() &&
        this->animation.isvalid() &&
        (enforceEvaluation || 
         this->frameDirty || 
         this->clipScheduler->IsDirty() ||
         this->firstEvaluation))
    {
        this->clipScheduler->Update(time);
         
        // at least 1 clip is currently active
        if (0 != this->clipScheduler->GetNumActiveClips())
        {       
            // get samples from current clip setup
            if (this->Sample(time, nCharacter2::keyArray, nCharacter2::scratchKeyArray, nCharacter2::MaxCurves))
            {               
                this->TransferSampledAnimToSkeleton();                 
            }
        }
        // first skeleton evaluation
        // -> play the first clip for character2s with only 1 clip
        // -> sample the first frame of the first clip for all other character2s
        else if (this->firstEvaluation && 0 != this->clipScheduler->GetNumClips())
        {
            this->clipScheduler->SetClipWeightAt(0, 1.0f);
            this->clipScheduler->Update(time);
            if (this->Sample(0.0, nCharacter2::keyArray, nCharacter2::scratchKeyArray, nCharacter2::MaxCurves))
            {
                this->TransferSampledAnimToSkeleton(); 
            }

            if(this->clipScheduler->GetNumClips() > 1)
            {
                this->clipScheduler->SetClipWeightAt(0, 0.0f);
            }
        }
        this->charSkeleton.Evaluate();
        this->firstEvaluation = false;            

        if (!enforceEvaluation)
        {
            // update evaluation flag
            this->frameDirty = false;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
nCharacter2::TransferSampledAnimToSkeleton()
{
    int numJoints = this->charSkeleton.GetNumJoints();
    const Math::float4* keyPtr = keyArray;

    vector translate;
    Math::quaternion rotate;
    vector scale;

    int jointIndex;
    for (jointIndex = 0; jointIndex < numJoints; jointIndex++)
    {
        // read sampled translation, rotation and scale
        translate.set(keyPtr->x(), keyPtr->y(), keyPtr->z());         keyPtr++;
        rotate.set(keyPtr->x(), keyPtr->y(), keyPtr->z(), keyPtr->w()); keyPtr++;
        scale.set(keyPtr->x(), keyPtr->y(), keyPtr->z());             keyPtr++;
        
        nCharJoint& joint = this->charSkeleton.GetJointAt(jointIndex);
        joint.SetTranslate(translate);
        joint.SetRotate(rotate);
        joint.SetScale(scale);
    }
}

//------------------------------------------------------------------------------
/**
    Sample the complete animation state at a given time into the
    provided float4 array. The size of the provided key array must be
    equal to the number of curves in any anim clip in the state (all clips
    must have the same number of curves).

    @param  time            the time at which to sample
    @param  keyArray        pointer to a float4 array which will be filled with 
                            the sampled values (one per curve)
    @param  keyArraySize    number of elements in the key array, must be identical 
                            to the number of curves in any anim clip
    @return                 true, if the returned keys are valid (false if all
                            clip weights are zero)
*/
bool
nCharacter2::Sample(Timing::Time time, Math::float4* keyArray, Math::float4* scratchKeyArray, int keyArraySize)
{
    n_assert(keyArray);
    n_assert(this->animation.isvalid());
    
     bool calculateCurveWeights = false;
    int firstAnimIndex = InvalidIndex;
    
    const float weightSum = this->clipScheduler->GetWeightSum();
    // if weightSum is 0.0f or smaller, cancel sampling...
    if(0.0f >= weightSum)
    {
        return false;
    }

    const int numClips = this->clipScheduler->GetNumClips();
    const Util::Array<int>& activeClipIndices = this->clipScheduler->GetActiveClipIndices();
    
    // scale weightAccum so that 1 == (weightAccum + weight)
    // Default: one weightAccum, Advanced: this->weightAccums[numCurves]
    float scaledWeight = 0.0f;
    float weightAccum = 0.0f;
    float clipWeight = 0.0f;
    
    // FIXME / HACK: advanced blending enforcement is required for complex animation blending 
    // with a lot of fast clip changes that would otherwise result in permanent blend mode switches 
    // and blend errors - enable the enforcement for lip/facial animations combined with other animations
    if(!this->forceAdvancedBlending)
    {
        //this->clipScheduler->IsDirty() has a bug with normal belnding mixed with advanced blending
        //it is possible this call will do the first frame animation problem

        // clip count or weighting changed since last sampling?
        //if(this->clipScheduler->IsDirty())
        {
            // Advanced blending, if:
            // (1) at least 2 active clips and
            // (2) at least 1 clip doesn't influence the complete skeleton
            if(this->IsAdvancedBlendingRequired())
            {
                calculateCurveWeights = true;
                this->blendMode = Advanced;
            }
            // Default blending, used for CompleteSkeleton clips
            else
            {
                this->blendMode = Default;
            }
        }
    }
    else
    {
        calculateCurveWeights = true;
        this->blendMode = Advanced;
    }

    // for each active clip ...
    uint activeClipIndex;
    for (activeClipIndex = 0; activeClipIndex < activeClipIndices.Size(); activeClipIndex++)
    {   
        const int clipIndex = activeClipIndices[activeClipIndex];
        clipWeight = this->clipScheduler->GetClipWeightAt(clipIndex) / weightSum;
        const Timing::Time clipTime = this->clipScheduler->GetClipTimeAt(clipIndex, time);
        const Timing::Time clipTimeOffset = this->clipScheduler->GetClipTimeOffsetAt(clipIndex);        
        const int animGroupIndex = this->clipScheduler->GetClipAnimGroupIndexAt(clipIndex);
        const nAnimation::Group& group = this->animation->GetGroupAt(animGroupIndex);
        const int numCurves = group.GetNumCurves();
        scaledWeight = clipWeight / (weightAccum + clipWeight);

        // obtain sampled curve value for the clip's anim curve range
        this->animation->SampleCurves(clipTime + clipTimeOffset, animGroupIndex, 0, numCurves, scratchKeyArray);

        // its the second active clip - store first active clip index now (used later for sample key lerping)
        if(1 == activeClipIndex)
        {
            firstAnimIndex = activeClipIndices[0];
        }
        
        // for each curve ...
        int curveIndex;
        for (curveIndex = 0; curveIndex < numCurves; curveIndex++)
        {   
            Math::float4& curMixedKey = keyArray[curveIndex];
            const Math::float4& curSampleKey = scratchKeyArray[curveIndex];
            nAnimation::Curve& animCurve = group.GetCurveAt(curveIndex);
            animCurve.SetConstValue(curSampleKey);

            // configure weights for Advanced blend mode
            if(Advanced == this->blendMode)
            {
                // its the first active clip - clear weightAccums array curveIndex by curveIndex
                if(0 == activeClipIndex)
                {
                    this->weightAccums[curveIndex] = 0.0f;
                }
                
                // calculate curve weight
                if(calculateCurveWeights)
                {
                    animCurve.SetCurveWeight(this->CalculateCurveWeight(clipIndex, curveIndex));
                }

                const float curveWeight = animCurve.GetCurveWeight();
                clipWeight = curveWeight;
                scaledWeight = curveWeight;
                
                // calculate scaled weight only for curveWeights > 0.0f
                if(0.0f < curveWeight)
                {
                    scaledWeight = curveWeight / (this->weightAccums[curveIndex] + curveWeight);
                }                
                this->weightAccums[curveIndex] += curveWeight;
            }
            
            // perform weighted blending
            this->PerformWeightedBlending(animCurve, curSampleKey, curMixedKey, curveIndex, firstAnimIndex, clipWeight, scaledWeight);
        }
        weightAccum += clipWeight;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
    Performs weighted blending with the given animation curve and sample key
    objects. Helper of nCharacter2::Sample()
    The mixedKey reference receives the result.

    @param  curve           the curve that is blended with the current blend result
    @param  sampleKey       reference to const Math::float4 containing a sampled key
    @param  mixedKey        reference to a Math::float4 that receives the blending result
    @param  curveIndex      the curve index of the current curve
    @param  firstAnimIndex  the clip index of the first active clip
    @param  clipWeight      the weighting of the current clip
    @param  scaledWeight    clipWeight / (weightAccum + clipWeight)
*/
void
nCharacter2::PerformWeightedBlending(const nAnimation::Curve& curve, const Math::float4& sampleKey, Math::float4& mixedKey, int curveIndex, int firstAnimIndex, float clipWeight, float scaledWeight)
{
    // some static helper objects
    static Math::quaternion quatCurrent;
    static Math::quaternion quatAccum;
    static Math::quaternion quatSlerp;
    
    // no previous curves are animated; take my current sample key
    if(InvalidIndex == firstAnimIndex)
    {   
        if (curve.GetIpolType() == nAnimation::Curve::Quat)
        {    
            mixedKey = sampleKey;
        }
        else
        {
            mixedKey = sampleKey * clipWeight;
        }
    }
    // take previous curves into account
    else
    {
        Math::float4 startValue = sampleKey;
        if (!curve.IsAnimated())
        {
            nAnimation::Curve& prevClipCurve = this->animation->GetGroupAt(firstAnimIndex).GetCurveAt(curveIndex);
            startValue = prevClipCurve.GetConstValue();
        }

        // lerp my current sample key with previous curves
        if (curve.GetIpolType() == nAnimation::Curve::Quat)
        {    
            quatCurrent.set(startValue.x(), startValue.y(), startValue.z(), startValue.w());
            quatAccum.set(mixedKey.x(), mixedKey.y(), mixedKey.z(), mixedKey.w());
            quatSlerp = Math::quaternion::slerp(quatAccum, quatCurrent, scaledWeight);
            mixedKey.set(quatSlerp.x(), quatSlerp.y(), quatSlerp.z(), quatSlerp.w());
        }
        else
        {
            mixedKey += startValue * clipWeight;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
nCharacter2::SetupClipScheduler(const Ptr<Models::CharacterNode>& charNode)
{
    n_assert(this->animation.isvalid());
    n_assert(!this->clipScheduler.isvalid());

    this->clipScheduler = nAnimClipScheduler::Create();

    int animGroupIndex;
    float clipDuration;
    float clipFadeTime;
    Util::String clipName;
    nAnimation::Group::LoopType clipLoopType;
    
    int numClips = charNode->GetNumClips();
    
    // setup the weightAccum array - once
    this->weightAccums.SetSize(this->animation->GetGroupAt(0).GetNumCurves());

    this->clipScheduler->BeginClips(numClips);

    int clipIndex;
    // for each clip
    for(clipIndex = 0; clipIndex < numClips; clipIndex++)
    {
        animGroupIndex = charNode->GetClipAt(clipIndex).GetAnimGroupIndex();
        clipDuration = (float) this->animation->GetGroupAt(animGroupIndex).GetDuration();
        clipFadeTime = this->animation->GetGroupAt(animGroupIndex).GetFadeInFrames() / animFramesPerSecond;
        clipLoopType = this->animation->GetGroupAt(animGroupIndex).GetLoopType(); 
        clipName = charNode->GetClipAt(clipIndex).GetClipName();

        nAnimClipScheduler::InfluenceType influence = nAnimClipScheduler::CompleteSkeleton;
        // determine influence type
        int curveIndex;
        for(curveIndex = 0; curveIndex < this->animation->GetGroupAt(animGroupIndex).GetNumCurves(); curveIndex++)
        {
            if(!this->animation->GetGroupAt(animGroupIndex).GetCurveAt(curveIndex).IsAnimated())
            {
                influence = nAnimClipScheduler::SelectedJoints;
                break;
            }
        }
        
        // set clip data on scheduler
        this->clipScheduler->SetClip(clipIndex, clipName, 0.0f, 0.0f, clipDuration, clipFadeTime, clipFadeTime, animGroupIndex, clipLoopType, influence);
    }

    this->clipScheduler->EndClips();
}

//------------------------------------------------------------------------------
/**
    Calculate and return the curve weight of a specific animation curve of an 
    animation clip
*/
float
nCharacter2::CalculateCurveWeight(int clipIndex, int curveIndex) const
{   
    float curveWeightSum = 0.0f;
    int numValidAnimatedCurvesAtIndex = 0; // valid animated curve: animated by a clip with a weight > 0.0f    
    IndexT animGroupIdx = this->clipScheduler->GetClipAnimGroupIndexAt(clipIndex);

    const int currentClipAnimatesCurve = this->animation->GetGroupAt(animGroupIdx).GetCurveAt(curveIndex).IsAnimated();
    const float currentClipWeight = this->clipScheduler->GetClipWeightAt(clipIndex);
    const Util::Array<int>& activeClipIndices = this->clipScheduler->GetActiveClipIndices();

    IndexT activeClipIdx;
    // calculate curveWeightSum and and count valid animated curves
    for(activeClipIdx = 0; activeClipIdx < activeClipIndices.Size(); activeClipIdx++)
    {
        animGroupIdx = this->clipScheduler->GetClipAnimGroupIndexAt(activeClipIndices[activeClipIdx]);        
        
        if(this->animation->GetGroupAt(animGroupIdx).GetCurveAt(curveIndex).IsAnimated())
        {
            const float clipWeight = this->clipScheduler->GetClipWeightAt(activeClipIndices[activeClipIdx]);
            curveWeightSum += clipWeight;

            if(clipWeight > 0.0f)
            {
                numValidAnimatedCurvesAtIndex++;
            }
        }
    }
    
    // the clip at 'clipIndex' has a valid weight and and it animates the curve
    if(0 != currentClipAnimatesCurve && currentClipWeight > 0.0f)
    {
        // there are others clips with a weight > 0.0f animating the curve?
        if(numValidAnimatedCurvesAtIndex > 1)
        {
            return currentClipWeight / curveWeightSum;
        }
        else
        {
            return 1.0f;
        }
    }
    // the clip does not animate the curve or the clip does not have a valid weight but it animates the curve
    if(0 == currentClipAnimatesCurve || (0 != currentClipAnimatesCurve && currentClipWeight <= 0.0f))
    {
        // no valid animated curves exist
        if(0 == numValidAnimatedCurvesAtIndex)
        {
            return 1.0f / activeClipIndices.Size();
        }
    }
    return 0.0f;
}

//------------------------------------------------------------------------------
/**
    Determine, if Advanced blending is required for the currrently active
    clip combination

    It is required, if:
    (1) at least 2 clips are active and 
    (2) one of them doesn't influence the complete skeleton
*/
bool
nCharacter2::IsAdvancedBlendingRequired() const
{
    const Util::Array<int>& activeClipIndices = this->clipScheduler->GetActiveClipIndices();
    const int numActiveClips = activeClipIndices.Size();

    if(1 < numActiveClips)
    {
        int index;
        for(index = 0; index < numActiveClips; index++)
        {
            if(nAnimClipScheduler::SelectedJoints == this->clipScheduler->GetClipInfluenceTypeAt(activeClipIndices[index]))
            {
                return true;
            }
        }
    }
    
    return false;
}

//------------------------------------------------------------------------------
/**
 	Get hotspots between two time stamps 
 	(use Grahics::Entity::GetEntityTime() for time stamps)
*/
uint
nCharacter2::GetHotspotsInRange(Timing::Time fromTime, Timing::Time toTime, Util::Array<nAnimation::Group::AnimHotspotInfo>& outResult)
{
    // not fully  initialitzed return 0 hotspots
    if (!this->clipScheduler.isvalid())
    {
        return 0;
    }
    const Util::Array<int>& activeClipIndices = this->clipScheduler->GetActiveClipIndices();
    const float weightSum = this->clipScheduler->GetWeightSum();

    uint hotspotCount = 0;

    // for each active clip ...
    uint activeClipIndex;
    for (activeClipIndex = 0; activeClipIndex < activeClipIndices.Size(); activeClipIndex++)
    {       
        const int clipIndex = activeClipIndices[activeClipIndex];
        const float clipWeight = this->clipScheduler->GetClipWeightAt(clipIndex) / weightSum;

        // compute time
        const Timing::Time clipTimeFrom = this->clipScheduler->GetClipTimeAt(clipIndex, (float)fromTime);
        const Timing::Time clipTimeTo = this->clipScheduler->GetClipTimeAt(clipIndex, (float)toTime);
        const Timing::Time clipTimeOffset = this->clipScheduler->GetClipTimeOffsetAt(clipIndex);

        // get animation group
        const int animGroupIndex = this->clipScheduler->GetClipAnimGroupIndexAt(clipIndex);
        const nAnimation* animation = this->GetAnimation();
        nAnimation::Group& animGroup = animation->GetGroupAt(animGroupIndex);

        // get hotspots from animation group
        IndexT idxHotspot = outResult.Size();
        hotspotCount += animGroup.GetHotspotsInRange(clipTimeFrom + clipTimeOffset, clipTimeTo + clipTimeOffset, outResult);
        
        // set weighting
        for (;idxHotspot < outResult.Size(); idxHotspot++)
        {
            outResult[idxHotspot].weight = clipWeight;
        }
    }

    return hotspotCount;
}
}; // namespace Nebula2