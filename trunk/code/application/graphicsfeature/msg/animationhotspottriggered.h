#ifndef MSG_ANIMATIONHOTSPOTTRIGGERED_H
#define MSG_ANIMATIONHOTSPOTTRIGGERED_H
//------------------------------------------------------------------------------
/**
    @class Messaging::AnimationHotspotTriggered
    
    (C) 2005 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "nebula2/anim/nanimation.h"
#include "util/array.h"

namespace Graphics
{
    class Entity;
}

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class AnimationHotspotTriggered : public Messaging::Message
{
    DeclareClass(AnimationHotspotTriggered);
    DeclareMsgId;

public:
    /// constructor
    AnimationHotspotTriggered();
    /// return triggered hotspot count
    SizeT GetNumHotspots() const;
    /// return the name of triggered hotspot by index
    const Util::String& GetHotspotNameAt(IndexT index) const;
    /// return the weight of the animation where the hotspot was triggered
    float GetHotspotWeightAt(IndexT index) const;
    /// return the time since hot spot was reached
    Timing::Time GetHotspotRunningSinceAt(IndexT index) const;

    /// get hotspot infos
    Util::Array<Nebula2::nAnimation::Group::AnimHotspotInfo>& Hotspots();

private:
    Util::Array<Nebula2::nAnimation::Group::AnimHotspotInfo> hotspots;
};

//------------------------------------------------------------------------------
/**
*/
inline
Util::Array<Nebula2::nAnimation::Group::AnimHotspotInfo>&
AnimationHotspotTriggered::Hotspots()
{
    return this->hotspots;
}

//------------------------------------------------------------------------------
/**
*/
inline
SizeT
AnimationHotspotTriggered::GetNumHotspots() const
{
    return this->hotspots.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
AnimationHotspotTriggered::GetHotspotNameAt(IndexT index) const
{
    n_assert(this->hotspots.Size() > index);
    n_assert(InvalidIndex < index);

    return this->hotspots[index].name;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
AnimationHotspotTriggered::GetHotspotWeightAt(IndexT index) const
{
    n_assert(this->hotspots.Size() > index);
    n_assert(InvalidIndex < index);

    return this->hotspots[index].weight;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
AnimationHotspotTriggered::GetHotspotRunningSinceAt(IndexT index) const
{
    n_assert(this->hotspots.Size() > index);
    n_assert(InvalidIndex < index);

    return this->hotspots[index].since;
}

}; // namespace Msg
//------------------------------------------------------------------------------
#endif