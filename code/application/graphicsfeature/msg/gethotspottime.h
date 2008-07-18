#ifndef MSG_GETHOTSPOTTIME_H
#define MSG_GETHOTSPOTTIME_H
//------------------------------------------------------------------------------
/**
    @class Messaging::GetHotspotTime

    Return the time of a hotspot
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class GetHotspotTime : public Messaging::Message
{
    DeclareClass(GetHotspotTime);
    DeclareMsgId;

public:
    /// constructor
    GetHotspotTime();
    /// destructor
    virtual ~GetHotspotTime();

    /// set name of animation
    void SetAnimationName(const Util::String& str);
    /// return name of animation
    const Util::String& GetAnimationName() const;

    /// set hotspot name
    void SetHotspotName(const Util::String& str);
    /// return hotspot name
    const Util::String& GetHotspotName() const;

    /// set hotspot time 
    void SetTime(Timing::Time t);
    /// return hotspot time
    Timing::Time GetTime() const;

private:
    Util::String animName;
    Util::String hotspotName;
    Timing::Time hotspotTime;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
GetHotspotTime::SetAnimationName(const Util::String& str)
{
    this->animName = str;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
GetHotspotTime::GetAnimationName() const
{
    return this->animName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetHotspotTime::SetHotspotName(const Util::String& str)
{
    this->hotspotName = str;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
GetHotspotTime::GetTime() const
{
    return this->hotspotTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetHotspotTime::SetTime(Timing::Time t)
{
    this->hotspotTime = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
GetHotspotTime::GetHotspotName() const
{
    return this->hotspotName;
}

}; // namespace Msg
//------------------------------------------------------------------------------
#endif