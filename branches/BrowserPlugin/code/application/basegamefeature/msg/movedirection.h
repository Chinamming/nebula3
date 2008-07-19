#ifndef MSG_MOVEDIRECTION_H
#define MSG_MOVEDIRECTION_H
//------------------------------------------------------------------------------
/**
    @class Messaging::MoveDirection
        
    A MoveDirection message. Expected behaviour is that the entity
    starts to move into the specified direction. The direction vector
    can be defined as camera relative or absolute. The velocity settings
    should be interpreted as a factor.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "math/vector.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class MoveDirection : public Messaging::Message
{
    DeclareClass(MoveDirection);
    DeclareMsgId;

public:
    /// constructor
    MoveDirection();
    /// set the direction vector
    void SetDirection(const Math::vector& v);
    /// get the direction vector
    const Math::vector& GetDirection() const;
    /// set maximum distance to move
    void SetMaxMovement(float m);
    /// get maximum distance to move
    float GetMaxMovement() const;
    /// set camera relative flag
    void SetCameraRelative(bool b);
    /// get camera relative flag
    bool GetCameraRelative() const;

private:
    Math::vector dir;
    bool camRelative;
    float maxMovement;
};

//------------------------------------------------------------------------------
/**
*/
inline MoveDirection::MoveDirection() :
    dir(1.0f, 0.0f, 0.0f),
    maxMovement(1.0f),
    camRelative(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
MoveDirection::SetDirection(const Math::vector& v)
{
    this->dir = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
MoveDirection::GetDirection() const
{
    return this->dir;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MoveDirection::SetMaxMovement(float m)
{
    this->maxMovement = m;
}

//------------------------------------------------------------------------------
/**
*/
inline float
MoveDirection::GetMaxMovement() const
{
    return this->maxMovement;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MoveDirection::SetCameraRelative(bool b)
{
    this->camRelative = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
MoveDirection::GetCameraRelative() const
{
    return this->camRelative;
}

}; // namespace BaseGameFeature
//------------------------------------------------------------------------------
#endif

