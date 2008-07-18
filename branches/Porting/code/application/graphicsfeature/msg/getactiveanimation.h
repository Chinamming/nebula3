#ifndef MSG_GETACTIVEANIMATION_H
#define MSG_GETACTIVEANIMATION_H
//------------------------------------------------------------------------------
/**
    @class Messaging::GetActiveAnimation
    
    Get base or overlay animation of an actor.
    
    (C) 2005 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class GetActiveAnimation : public Messaging::Message
{
    DeclareClass(GetActiveAnimation);
    DeclareMsgId;

public:
    /// constructor
    GetActiveAnimation();
    /// set to true to get the base animation name
    void RequestBaseAnim(bool base);
    /// return true if base animation is requestet
    bool IsBaseAnimRequested() const;
    /// set true to get the active clip name as result
    void RequestClipName(bool b);
    /// return true if clip name is requested
    bool IsClipNameRequested() const;
    /// set the active animation name
    void SetAnimName(const Util::String& anim);
    /// return the name of the active animation
    const Util::String& GetAnimName() const;
    /// return true if a active animation is set
    bool HasActiveAnim() const;

private:
    bool baseAnim;
    bool clipName;
    Util::String animName;
};

//------------------------------------------------------------------------------
/**
*/
inline
GetActiveAnimation::GetActiveAnimation() :
    baseAnim(false),
    clipName(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetActiveAnimation::RequestBaseAnim(bool base)
{
    this->baseAnim = base;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
GetActiveAnimation::IsBaseAnimRequested() const
{
    return this->baseAnim;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetActiveAnimation::RequestClipName(bool b)
{
    this->clipName = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
GetActiveAnimation::IsClipNameRequested() const
{
    return this->clipName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetActiveAnimation::SetAnimName(const Util::String& anim)
{
    this->animName = anim;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
GetActiveAnimation::GetAnimName() const
{
    return this->animName;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
GetActiveAnimation::HasActiveAnim() const
{
    return this->animName.IsValid();
}

}; // namespace Msg
//------------------------------------------------------------------------------
#endif
