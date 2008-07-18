#ifndef MSG_UPDATETRANSFORM_H
#define MSG_UPDATETRANSFORM_H
//------------------------------------------------------------------------------
/**
    @class Messaging::UpdateTransform
    
    Updates the transform of a entity, does not set the physics tranform.
    All property that need to update when entity transformation changes need
    listen to this messags.

    To set the transformation of a entity (including the physics) use the SetTransform message.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class UpdateTransform : public Messaging::Message
{
	DeclareClass(UpdateTransform);
    DeclareMsgId;

public:
    /// constructor
    UpdateTransform();
    /// set the target transform matrix
    void SetMatrix(const Math::matrix44& m);
    /// get the target transform matrix
    const Math::matrix44& GetMatrix() const;
    /// set smoothing flag
    void SetSmoothing(bool b);
    /// get smoothing flag
    bool GetSmoothing() const;

private:
    Math::matrix44 transform;
    bool smoothing;
};

//------------------------------------------------------------------------------
/**
*/
inline UpdateTransform::UpdateTransform():
    smoothing(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
UpdateTransform::SetMatrix(const Math::matrix44& m)
{
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
UpdateTransform::GetMatrix() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
UpdateTransform::SetSmoothing(bool b)
{
    this->smoothing = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
UpdateTransform::GetSmoothing() const
{
    return this->smoothing;
}

}; // namespace BaseGameFeature
//------------------------------------------------------------------------------
#endif
