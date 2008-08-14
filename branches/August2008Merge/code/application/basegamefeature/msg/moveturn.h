#ifndef MSG_MOVETURN_H
#define MSG_MOVETURN_H
//------------------------------------------------------------------------------
/**
    @class Messaging::MoveTurn

    Commands an entity to turn into the specified direction defined
    by a 3d vector. The direction vector can be absolute or camera relative.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "game/entity.h"
#include "math/vector.h"
#include "basegameattr/basegameattributes.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class MoveTurn : public Messaging::Message
{
	DeclareClass(MoveTurn);
    DeclareMsgId;

public:
    /// constructor
    MoveTurn();
    /// set the direction vector
    void SetDirection(const Math::vector& v);
    /// get the direction vector
    const Math::vector& GetDirection();
    /// set target entity
    void SetEntities(const Ptr<Game::Entity>& entity, const Ptr<Game::Entity>& target);
    /// set camera relative flag
    void SetCameraRelative(bool b);
    /// get camera relative flag
    bool GetCameraRelative() const;

private:
    Math::vector dir;
    bool camRelative;
    Ptr<Game::Entity> targetEntity;
    Ptr<Game::Entity> entity;
};

//------------------------------------------------------------------------------
/**
*/
inline void
MoveTurn::SetDirection(const Math::vector& v)
{
    this->dir = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
MoveTurn::GetDirection()
{
    if (this->targetEntity.isvalid() && this->entity.isvalid())
    {
        this->camRelative = false;
        const Math::vector targetPos = this->targetEntity->GetMatrix44(Attr::Transform).getrow3();
        const Math::vector entityPos = this->entity->GetMatrix44(Attr::Transform).getrow3();
        this->dir = targetPos - entityPos;
        this->dir = Math::vector::normalize(this->dir);
        return this->dir;
    }
    else
    {
        return this->dir;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void
MoveTurn::SetCameraRelative(bool b)
{
    this->camRelative = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
MoveTurn::GetCameraRelative() const
{
    return this->camRelative;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MoveTurn::SetEntities(const Ptr<Game::Entity>& entity, const Ptr<Game::Entity>& target)
{
    this->entity = entity;
    this->targetEntity = target;
}

}; // namespace BaseGameFeature
//------------------------------------------------------------------------------
#endif

