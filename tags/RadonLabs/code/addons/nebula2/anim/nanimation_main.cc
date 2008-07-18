//------------------------------------------------------------------------------
//  nanimation_main.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nanimation.h"
#include "math/scalar.h"

namespace Nebula2
{
ImplementClass(Nebula2::nAnimation, 'NANI' ,Resources::Resource);
//------------------------------------------------------------------------------
/**
*/
nAnimation::nAnimation()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
nAnimation::~nAnimation()
{
    if (this->IsLoaded())
    {
        this->Unload();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
nAnimation::UnloadResource()
{
    this->groupArray.SetSize(0);
}

//------------------------------------------------------------------------------
/**
    This method should be overwritten by subclasses.
*/
void 
nAnimation::SampleCurves(Timing::Time /*time*/, int /*groupIndex*/, int /*firstCurveIndex*/, int /*numCurves*/, Math::float4* /*keyArray*/)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Builds hotspots from mata data
*/
void
nAnimation::Group::BuildHotspots()
{
    if (0)//this->metaData.HasData("Hotspot"))
    {
        // hotspot meta data format name1#time1>name2#time2
        const Util::Variant& variant = this->metaData.GetData("Hotspot");
        Util::Array<Util::String> hotpots = variant.GetString().Tokenize(">");
        
        uint idxHotspot;
        for (idxHotspot = 0; idxHotspot < hotpots.Size(); idxHotspot++)
        {
            Util::Array<Util::String> hotspot = hotpots[idxHotspot].Tokenize("#");
            Util::String name;
            float time;

            // fallback
            if (1 == hotspot.Size() && hotspot[0].IsValidFloat())
            {
                name = "default";
                time = hotspot[0].AsFloat();
            }
            else if (2 == hotspot.Size() && hotspot[1].IsValidFloat())
            {
                name = hotspot[0];
                time = hotspot[1].AsFloat();

                if (name == "left_feet_1")
                {
                    int i = 0;
                }
            }
            else
            {
                n_error("nAnimation::Group::BuildHotspots() Hotspot has wrong format!\n");
            }

            // append hotspot to frame key
            IndexT timeIndex = this->hotspotsByFrame.FindIndex(time);
            if (InvalidIndex != timeIndex)
            {
                this->hotspotsByFrame.ValueAtIndex(timeIndex).Append(name);
            }
            else
            {
                Util::Array<Util::String> names;
                names.Append(name);
                this->hotspotsByFrame.Add(time, names);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    return hotspot count between from and to time
    appends AnimHotspotInfo to outResult 
*/
uint
nAnimation::Group::GetHotspotsInRange(Timing::Time from, Timing::Time to, Util::Array<AnimHotspotInfo>& outResult) const
{
    //n_assert(0 <= from);
    //n_assert(0 <= to);

    // time between 0 and duration
    from = fmod(from, this->GetDuration());
    to = fmod(to, this->GetDuration());

    // find first and last hotspot index
    IndexT idxFrom = this->FindHotspotIndex(from / this->keyTime);
    IndexT idxTo = this->FindHotspotIndex(to / this->keyTime);

    // append hotspot infos
    SizeT result = 0;
    if (idxFrom <= idxTo)
    {
        // default case:
        IndexT idx;
        for (idx = idxFrom; idx < idxTo; idx++)
        {
            this->GetHotspotsByIndex(idx, outResult);
        }

        result = idxTo - idxFrom;
    }
    else
    {
        // special case:
        // reached the end of the animation (loop)
        IndexT idx;
        for (idx = idxFrom; idx < this->hotspotsByFrame.Size(); idx++)
        {
            this->GetHotspotsByIndex(idx, outResult);
        }

        for (idx = 0; idx < idxTo; idx++)
        {
            this->GetHotspotsByIndex(idx, outResult);
        }

        result = (this->hotspotsByFrame.Size() - idxFrom) + idxTo;
    }

    return result;
}


//------------------------------------------------------------------------------
/**
    return the index of the last hotspot that frame is lower then the given
*/
IndexT
nAnimation::Group::FindHotspotIndex(Timing::Time frame, IndexT from) const
{
    IndexT result = from;
    while (result < this->hotspotsByFrame.Size() &&
        this->hotspotsByFrame.KeyAtIndex(result) < frame)
    {
        result++;
    }

    return result;
}

//------------------------------------------------------------------------------
/**
    appends and hotspot info struct to the outResult array
*/
void
nAnimation::Group::GetHotspotsByIndex(IndexT index, Util::Array<AnimHotspotInfo>& outResult) const
{
    const Util::Array<Util::String>& hotspotNames = this->hotspotsByFrame.ValueAtIndex(index);
    const Timing::Time& hotspotTime = this->hotspotsByFrame.KeyAtIndex(index) / this->keyTime;

    IndexT idxHotspot;
    for (idxHotspot = 0; idxHotspot < hotspotNames.Size(); idxHotspot++)
    {
        AnimHotspotInfo hotspot;
        hotspot.name = hotspotNames[idxHotspot];
        hotspot.since = 0.f;
        hotspot.weight = 1.f;
        if (hotspot.since < 0)
        {
            hotspot.since = this->GetDuration() - hotspot.since;
        }

        outResult.Append(hotspot);
    }
}

//------------------------------------------------------------------------------
/**
    - 18-Oct-04   floh      fixed case where returned indices could be <0
*/
void
nAnimation::Group::TimeToIndex(Timing::Time time, int& keyIndex0, int& keyIndex1, Timing::Time& inbetween) const
{
    Timing::Time frame  = time / this->keyTime;
    int intFrame = int(frame);
    keyIndex0    = intFrame - this->startKey;
    keyIndex1    = keyIndex0 + 1;
    inbetween    = Timing::Time(Math::n_saturate(float(frame - Timing::Time(intFrame))));
    if (Clamp == this->loopType)
    {
        // 'clamp' loop type
        if (keyIndex0 < 0)                   keyIndex0 = 0;
        else if (keyIndex0 >= this->numKeys) keyIndex0 = this->numKeys - 1;
        if (keyIndex1 < 0)                   keyIndex1 = 0;
        else if (keyIndex1 >= this->numKeys) keyIndex1 = this->numKeys - 1;
    }
    else
    {
        // 'repeat' loop type
        keyIndex0 %= this->numKeys;
        keyIndex1 %= this->numKeys;
        if (keyIndex0 < 0)
        {
            keyIndex0 += this->numKeys;
        }
        if (keyIndex1 < 0)
        {
            keyIndex1 += this->numKeys;
        }
    }
    n_assert((keyIndex0 >= 0) && (keyIndex0 < this->numKeys));
    n_assert((keyIndex1 >= 0) && (keyIndex1 < this->numKeys));

    keyIndex0 *= this->keyStride;
    keyIndex1 *= this->keyStride;
}
}; // namespace Nebula2