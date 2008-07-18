#ifndef MSG_GETANIMATIONINFO_H
#define MSG_GETANIMATIONINFO_H
//------------------------------------------------------------------------------
/**
    @class Messaging::GetAnimationInfo
    
    Get animation related information.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class GetAnimationInfo : public Messaging::Message
{
    DeclareClass(GetAnimationInfo);
    DeclareMsgId;

public:
    /// constructor
    GetAnimationInfo();

    /// set an animation name
    void SetAnimationName(const Util::String& name);
    /// get the animation name
    const Util::String& GetAnimationName() const;

    /// set animation weight
    void SetWeight(float weight);
    /// get animation weight
    float GetWeight() const;

    /// set animation duration
    void SetDuration(Timing::Time duration);
    /// get animation duration
    Timing::Time GetDuration() const;

    /// set animation fade in time
    void SetFadeInTime(Timing::Time time);
    /// get animation fade in time
    Timing::Time GetFadeInTime() const;

    /// set animation fade out time
    void SetFadeOutTime(Timing::Time time);
    /// get animation fade out time
    Timing::Time GetFadeOutTime() const;

    /// set animation run time
    void SetRunTime(Timing::Time numberOfRuns);
    /// get animation run time
    Timing::Time GetRunTime() const;

    /// set animation sample time
    void SetSampleTime(Timing::Time time);
    /// get animation sample time
    Timing::Time GetSampleTime() const;

    /// set animation time factor
    void SetTimeFactor(float time);
    /// get animation time factor
    float GetTimeFactor() const;

private:
    Timing::Time fadeInTime;
    Timing::Time fadeOutTime;
    Timing::Time sampleTime;
    Timing::Time duration;
    float weight;
    Timing::Time runTime;
    float timeFactor;
    Util::String name;
};

//------------------------------------------------------------------------------
/**
*/
inline
GetAnimationInfo::GetAnimationInfo()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetAnimationInfo::SetAnimationName(const Util::String& name)
{
    this->name = name;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
GetAnimationInfo::GetAnimationName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetAnimationInfo::SetWeight(float weight)
{
    this->weight = weight;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
GetAnimationInfo::GetWeight() const
{
    return this->weight;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetAnimationInfo::SetDuration(Timing::Time duration)
{
    this->duration = duration;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
GetAnimationInfo::GetDuration() const
{
    return this->duration;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetAnimationInfo::SetFadeInTime(Timing::Time time)
{
    this->fadeInTime = time;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
GetAnimationInfo::GetFadeInTime() const
{
    return this->fadeInTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetAnimationInfo::SetFadeOutTime(Timing::Time time)
{
    this->fadeOutTime = time;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
GetAnimationInfo::GetFadeOutTime() const
{
    return this->fadeOutTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetAnimationInfo::SetRunTime(Timing::Time numberOfRuns)
{
    this->runTime = numberOfRuns;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time 
GetAnimationInfo::GetRunTime() const
{
    return this->runTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetAnimationInfo::SetSampleTime(Timing::Time time)
{
    this->sampleTime = time;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
GetAnimationInfo::GetSampleTime() const
{
    return this->sampleTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetAnimationInfo::SetTimeFactor(float t)
{
    this->timeFactor = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
GetAnimationInfo::GetTimeFactor() const
{
    return this->timeFactor;
}

}; // namespace Msg
//------------------------------------------------------------------------------
#endif
