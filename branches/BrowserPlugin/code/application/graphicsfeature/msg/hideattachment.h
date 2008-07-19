#ifndef MSG_HIDEATTACHMENT_H
#define MSG_HIDEATTACHMENT_H
//------------------------------------------------------------------------------
/**
    @class Messaging::HideAttachment
    
    Hide one or more attachment graphics entities. This doesn't
    remove the attachment, just set the graphics entity to hidden! The
    same filter rules apply as in RemAttachments.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "graphics/modelentity.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class HideAttachment : public Messaging::Message
{
    DeclareClass(HideAttachment);
    DeclareMsgId;
public:
    /// constructor
    HideAttachment();
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
};

//------------------------------------------------------------------------------
/**
*/
inline
HideAttachment::HideAttachment() :
    graphicsEntityType(Graphics::GraphicsEntity::InvalidType)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
HideAttachment::SetJointName(const Util::String& n)
{
    this->jointName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
HideAttachment::GetJointName() const
{
    return this->jointName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
HideAttachment::SetResourceName(const Util::String& n)
{
    this->resName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
HideAttachment::GetResourceName() const
{
    return this->resName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
HideAttachment::SetGraphicsEntityType(Graphics::GraphicsEntity::Type t)
{
    this->graphicsEntityType = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Graphics::GraphicsEntity::Type
HideAttachment::GetGraphicsEntityType() const
{
    return this->graphicsEntityType;
}

} // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif
    