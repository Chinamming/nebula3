#ifndef MSG_REMATTACHMENT_H
#define MSG_REMATTACHMENT_H
//------------------------------------------------------------------------------
/**
    @class Messaging::RemAttachment

    Remove an attachment from a joint.
    
    (C) 2005 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "graphics/modelentity.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class RemAttachment : public Messaging::Message
{
    DeclareClass(RemAttachment);
    DeclareMsgId;

public:
    /// constructor
    RemAttachment();
    /// set joint name, consider all joint if none given
    void SetJointName(const Util::String& n);
    /// get joint name
    const Util::String& GetJointName() const;
    /// set graphics resource name, consider all attachments if none given
    void SetResourceName(const Util::String& n);
    /// get graphics resource name
    const Util::String& GetResourceName() const;
    /// set graphics entity type, consider all types if none given
    void SetGraphicsEntityType(Graphics::ModelEntity::Type t);
    /// get optional graphics entity type
    Graphics::ModelEntity::Type GetGraphicsEntityType() const;
    /// set as alternative the graphics entity
    void SetGraphicsEntity(Graphics::ModelEntity* Entity);
    /// get the alternative graphics entity (may return 0)
    Graphics::ModelEntity* GetGraphicsEntity();

private:
    Util::String jointName;
    Util::String resName;
    Graphics::ModelEntity::Type graphicsEntityType;
    Ptr<Graphics::ModelEntity> Entity;
};

//------------------------------------------------------------------------------
/**
*/
inline
RemAttachment::RemAttachment() :
    graphicsEntityType(Graphics::ModelEntity::InvalidType)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
RemAttachment::SetJointName(const Util::String& n)
{
    this->jointName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
RemAttachment::GetJointName() const
{
    return this->jointName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
RemAttachment::SetResourceName(const Util::String& n)
{
    this->resName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
RemAttachment::GetResourceName() const
{
    return this->resName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RemAttachment::SetGraphicsEntityType(Graphics::ModelEntity::Type t)
{
    this->graphicsEntityType = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Graphics::ModelEntity::Type
RemAttachment::GetGraphicsEntityType() const
{
    return this->graphicsEntityType;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
RemAttachment::SetGraphicsEntity(Graphics::ModelEntity* Entity)
{
    this->Entity = Entity;
}

//------------------------------------------------------------------------------
/**
    NOTE: this method may return 0.
*/
inline
Graphics::ModelEntity*
RemAttachment::GetGraphicsEntity()
{
    return this->Entity.get_unsafe();
}

} // namespace Msg
//------------------------------------------------------------------------------
#endif
