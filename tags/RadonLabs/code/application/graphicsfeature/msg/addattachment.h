#ifndef MSG_ADDATTACHMENT_H
#define MSG_ADDATTACHMENT_H
//------------------------------------------------------------------------------
/**
    @class Messaging::AddAttachment

    Attach a graphics entity defined by a resource name to a joint.
    
    (C) 2005 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "graphics/modelentity.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class AddAttachment : public Messaging::Message
{
    DeclareClass(AddAttachment);
    DeclareMsgId;

public:
    /// constructor
    AddAttachment();

    /// set joint name
    void SetJointName(const Util::String& n);
    /// get joint name
    const Util::String& GetJointName() const;
    /// set graphics resource name
    void SetResourceName(const Util::String& n);
    /// get graphics resource name
    const Util::String& GetResourceName() const;
    /// set as alternative the graphics entity
    void SetGraphicsEntity(Graphics::ModelEntity* Entity);
    /// get the alternative graphics entity (may return 0)
    Graphics::ModelEntity* GetGraphicsEntity();
    /// set optional offset matrix
    void SetOffsetMatrix(const Math::matrix44& m);
    /// get offset matrix
    const Math::matrix44& GetOffsetMatrix() const;
    /// set if current attachments on the affected joint should be removed
    void SetRemCurAttachment(bool rem);
    /// get if current attachments on the affected joint should be removed
    bool GetRemCurAttachment() const;
    /// should graphics entity be attached to graphics level?
    void SetAttachEntityToLevel(bool b);
    /// get attach-to-level flag
    bool GetAttachEntityToLevel() const;
    /// set graphics to character scene
    void SetCharGraphics(bool b);
    /// return true if gaphics is a char
    bool IsCharGraphics() const;
    /// set animation mapping (anims.xml column)
    void SetAnimationMapping(const Util::String& str);
    /// get animation mapping (anims.xml column)
    const Util::String& GetAnimationMapping() const;

private:
    Util::String jointName;
    Util::String resName;
    Ptr<Graphics::ModelEntity> Entity;
    Math::matrix44 offsetMatrix;
    bool remCurAttachments;
    bool attachEntityToLevel;
    bool isCharGraphics;
    Util::String animMapping;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
AddAttachment::SetRemCurAttachment(bool rem)
{
    this->remCurAttachments = rem;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
AddAttachment::GetRemCurAttachment() const
{
    return this->remCurAttachments;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
AddAttachment::SetJointName(const Util::String& n)
{
    this->jointName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
AddAttachment::GetJointName() const
{
    return this->jointName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
AddAttachment::SetResourceName(const Util::String& n)
{
    this->resName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
AddAttachment::GetResourceName() const
{
    return this->resName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
AddAttachment::SetGraphicsEntity(Graphics::ModelEntity* Entity)
{
    this->Entity = Entity;
}

//------------------------------------------------------------------------------
/**
    NOTE: this method may return 0.
*/
inline
Graphics::ModelEntity*
AddAttachment::GetGraphicsEntity()
{
    return this->Entity.get_unsafe();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
AddAttachment::SetOffsetMatrix(const Math::matrix44& m)
{
    this->offsetMatrix = m;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::matrix44&
AddAttachment::GetOffsetMatrix() const
{
    return this->offsetMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AddAttachment::SetAttachEntityToLevel(bool b)
{
    this->attachEntityToLevel = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AddAttachment::GetAttachEntityToLevel() const
{
    return this->attachEntityToLevel;
}

//------------------------------------------------------------------------------
/**
*/
inline void
AddAttachment::SetCharGraphics(bool b)
{
    this->isCharGraphics = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AddAttachment::IsCharGraphics() const
{
    return this->isCharGraphics;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
AddAttachment::SetAnimationMapping(const Util::String& str)
{
    this->animMapping = str;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
AddAttachment::GetAnimationMapping() const
{
    return this->animMapping;
}

}; // namespace Msg
//------------------------------------------------------------------------------
#endif