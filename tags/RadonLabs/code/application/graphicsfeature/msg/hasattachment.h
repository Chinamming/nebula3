#ifndef MSG_HASATTACHMENT_H
#define MSG_HASATTACHMENT_H
//------------------------------------------------------------------------------
/**
    @class Messaging::HasAttachment
  
    Checks whether an attachment exists on the entity. The same
    filtering rules apply as RemAttachment.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "graphics/modelentity.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class HasAttachment : public Messaging::Message
{
    DeclareClass(HasAttachment);
    DeclareMsgId;
public:
    /// constructor
    HasAttachment();
    /// set joint name, consider all joint if none given
    void SetJointName(const Util::String& n);
    /// get joint name
    const Util::String& GetJointName() const;
    /// set graphics resource name, consider all attachments if none given
    void SetResourceName(const Util::String& n);
    /// get graphics resource name
    const Util::String& GetResourceName() const;
    /// set graphics entity type, consider all types if none given
    void SetGraphicsEntityType(Graphics::GraphicsEntity::Type t);
    /// get optional graphics entity type
    Graphics::GraphicsEntity::Type GetGraphicsEntityType() const;
    /// set the result
    void SetResult(bool b);
    /// get result
    bool GetResult() const;

private:
    Util::String jointName;
    Util::String resName;
    Graphics::GraphicsEntity::Type graphicsEntityType;
    bool result;
};

//------------------------------------------------------------------------------
/**
*/
inline
HasAttachment::HasAttachment() :
    graphicsEntityType(Graphics::GraphicsEntity::InvalidType),
    result(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
HasAttachment::SetJointName(const Util::String& n)
{
    this->jointName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
HasAttachment::GetJointName() const
{
    return this->jointName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
HasAttachment::SetResourceName(const Util::String& n)
{
    this->resName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
HasAttachment::GetResourceName() const
{
    return this->resName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
HasAttachment::SetGraphicsEntityType(Graphics::GraphicsEntity::Type t)
{
    this->graphicsEntityType = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Graphics::GraphicsEntity::Type
HasAttachment::GetGraphicsEntityType() const
{
    return this->graphicsEntityType;
}

//------------------------------------------------------------------------------
/**
*/
inline void
HasAttachment::SetResult(bool b)
{
    this->result = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
HasAttachment::GetResult() const
{
    return this->result;
}

} // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif
