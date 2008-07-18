#ifndef MSG_APPLYIMPULSEATPOS_H
#define MSG_APPLYIMPULSEATPOS_H
//------------------------------------------------------------------------------
/**
    @class Messaging::ApplyImpulseAtPos
        
    Apply an impulse vector at a position in the global coordinate frame to the
    physics entity of the game entity
    
    (C) 2006 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "math/vector.h"

//------------------------------------------------------------------------------
namespace PhysicsFeature
{
class ApplyImpulseAtPos : public Messaging::Message
{
    DeclareClass(ApplyImpulseAtPos);
    DeclareMsgId;

public:
    /// constructor
    ApplyImpulseAtPos();
    /// set the impulse vector
    void SetImpulse(const Math::vector& v);
    /// get the impulse vector
    const Math::vector& GetImpulse() const;
    /// set position
    void SetPosition(const Math::vector& v);
    /// get position
    const Math::vector& GetPosition() const;
    /// set multiply by mass
    void SetMultiplyByMass(bool b);
    /// get multiply by mass
    bool GetMultiplyByMass() const;

private:
    Math::vector impulse;
    Math::vector position;
    bool    multByMass;
};

//------------------------------------------------------------------------------
/**
*/
inline
ApplyImpulseAtPos::ApplyImpulseAtPos() :
    impulse(0.0f, 0.0f, 0.0f),
    position(0.0f, 0.0f, 0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ApplyImpulseAtPos::SetPosition(const Math::vector& v)
{
    this->position = v;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector&
ApplyImpulseAtPos::GetPosition() const
{
    return this->position;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
ApplyImpulseAtPos::SetImpulse(const Math::vector& v)
{
    this->impulse = v;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector&
ApplyImpulseAtPos::GetImpulse() const
{
    return this->impulse;
}


//------------------------------------------------------------------------------
/**
*/
inline
void
ApplyImpulseAtPos::SetMultiplyByMass(bool b)
{
    this->multByMass = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
ApplyImpulseAtPos::GetMultiplyByMass() const
{
    return this->multByMass;
}


}; // namespace Msg
//------------------------------------------------------------------------------
#endif

