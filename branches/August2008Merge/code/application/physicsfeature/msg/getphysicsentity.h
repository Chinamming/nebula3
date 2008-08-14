#ifndef MESSAGING_GETPHYSICSENTITY_H
#define MESSAGING_GETPHYSICSENTITY_H
//------------------------------------------------------------------------------
/**
    @class Messaging::GetPhysicsEntity

    Returns the Physics entity used by a game entity.

    (C) 2006 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "physics/physicsentity.h"

//------------------------------------------------------------------------------
namespace PhysicsFeature
{
class GetPhysicsEntity : public Messaging::Message
{
    DeclareClass(GetPhysicsEntity);
    DeclareMsgId;
public:
    /// set a single Physics entity
    void SetEntity(Physics::PhysicsEntity* e);
    /// get Physics entities array
    const Ptr<Physics::PhysicsEntity>& GetEntity() const;

private:
    Ptr<Physics::PhysicsEntity> entity;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
GetPhysicsEntity::SetEntity(Physics::PhysicsEntity* e)
{
    n_assert(0 != e);
    this->entity = e;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Ptr<Physics::PhysicsEntity>&
GetPhysicsEntity::GetEntity() const
{
    return this->entity;
}

} // namespace Messaging
//------------------------------------------------------------------------------
#endif

