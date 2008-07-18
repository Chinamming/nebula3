#ifndef MSG_MOVEGOTO_H
#define MSG_MOVEGOTO_H
//------------------------------------------------------------------------------
/**
    @class Messaging::MoveGoto

    A highlevel MoveGoto Messaging. The expected behaviour is that an entity
    should move to the target point defined by the move goto Messaging
    and stop there. If possible, the entity should steer around obstacles
    and use pathfinding to reach the target point.
    
    (C) 2005 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "math/vector.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class MoveGoto : public Messaging::Message
{
    DeclareClass(MoveGoto);
    DeclareMsgId;

public:
    /// constructor
    MoveGoto();
    /// set the target position
    void SetPosition(const Math::vector& p);
    /// get the target position
    const Math::vector& GetPosition() const;
    /// set the distance to target in which entity stops
    void SetDistance(float f);
    /// get the distance to target in which entity stops
    float GetDistance() const;

private:
    Math::vector pos;
    float distance;
};

//------------------------------------------------------------------------------
/**
*/
inline
MoveGoto::MoveGoto():
    distance(0.5f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
MoveGoto::SetPosition(const Math::vector& p)
{
    this->pos = p;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector&
MoveGoto::GetPosition() const
{
    return this->pos;
}
//------------------------------------------------------------------------------
/**
*/
inline
void 
MoveGoto::SetDistance(float f)
{
    this->distance = f;
}

//------------------------------------------------------------------------------
/**
*/
inline
float 
MoveGoto::GetDistance() const
{
    return this->distance;
}

}; // namespace BaseGameFeature
//------------------------------------------------------------------------------
#endif
