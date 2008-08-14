#ifndef MSG_SHOWATTACHMENT_H
#define MSG_SHOWATTACHMENT_H
//------------------------------------------------------------------------------
/**
    @class Messaging::ShowAttachment
    
    Hide one or more attachment graphics entities. This doesn't
    remove the attachment, just set the graphics entity to hidden! The
    same filter rules apply as in RemAttachments.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "graphics/graphicsentity.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class ShowAttachment : public Messaging::Message
{
    DeclareClass(ShowAttachment);
    DeclareMsgId;
public:
    /// constructor
    ShowAttachment();
    /// set joint name, consider all joint if none given
    void SetJointName(const Util::String& n);
    /// get joint name
    const Util::String& GetJointName() const;
    /// set graphics resource name, consider all attachments if none given
    void SetResourceName(const Util::String& n);
    /// get graphics resource name
    const Util::String& GetResourceName() const;
    /// set the result
    void SetResult(bool b);
    /// get result
    bool GetResult() const;
    /// set graphics entity type, consider all types if none given
    void SetGraphicsEntityType(Graphics::GraphicsEntity::Type t);
    /// get optional graphics entity type
    Graphics::GraphicsEntity::Type GetGraphicsEntityType() const;

private:
    Util::String jointName;
    Util::String resName;
    Graphics::GraphicsEntity::Type graphicsEntityType;
};

//------------------------------------------------------------------------------
/**
*/
inline
ShowAttachment::ShowAttachment() :
    graphicsEntityType(Graphics::GraphicsEntity::InvalidType)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShowAttachment::SetJointName(const Util::String& n)
{
    this->jointName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
ShowAttachment::GetJointName() const
{
    return this->jointName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShowAttachment::SetResourceName(const Util::String& n)
{
    this->resName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
ShowAttachment::GetResourceName() const
{
    return this->resName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShowAttachment::SetGraphicsEntityType(Graphics::GraphicsEntity::Type t)
{
    this->graphicsEntityType = t;
}

//------------------------------------------------------------------------------
/**
*/
inline Graphics::GraphicsEntity::Type
ShowAttachment::GetGraphicsEntityType() const
{
    return this->graphicsEntityType;
}

} // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif
    