#ifndef UTIL_CHARENHANCEMENTUTIL_H
#define UTIL_CHARENHANCEMENTUTIL_H
//------------------------------------------------------------------------------
/**
    @class Util::CharEnhancementUtil
    
    Manage graphics character entity attachments and layered skins.
   
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/dictionary.h"
#include "graphics/actorentity.h"
#include "game/entity.h"

//------------------------------------------------------------------------------

namespace GraphicsFeature
{
class SetAnimation;
class FadeAnimation;

class CharEnhancementUtil : public Core::RefCounted
{
    DeclareClass(CharEnhancementUtil);
public:
    /// Constructor
    CharEnhancementUtil();
    /// Destructor
    virtual ~CharEnhancementUtil();

    /// set the target graphics entity
    void SetCharacterGraphicsEntity(Graphics::ActorEntity* entity);
    /// get the target entity
    Graphics::ActorEntity* GetCharacterGraphicsEntity() const;

    /// add a game entity as attachment
    void AddAttachment(const Util::String& jointName, Game::Entity* entity, const Math::matrix44& offset);
    /// add an attachment by resource name or graphics entity pointer
    void AddAttachment(const Util::String& jointName, const Util::String& resName, Graphics::ModelEntity* graphicsEntity, const Math::matrix44& offset, bool attachEntity = true, bool charEntity = false);
    /// add an attachment by resource name or graphics entity pointer with animation mapping
    void AddAttachment(const Util::String& jointName, const Util::String& resName, Graphics::ModelEntity* graphicsEntity, const Math::matrix44& offset, const Util::String& animMapping, bool attachEntity = true, bool charEntity = false);
    /// remove attachments by joint name, resource name, graphics entity pointer, and/or graphics entity type
    void RemoveAttachments(const Util::String& jointName, const Util::String& resName, Graphics::ModelEntity* graphicsEntity, Graphics::ModelEntity::Type graphicsEntityType=Graphics::ModelEntity::InvalidType);
    /// return true if a joint has at least one attachment matching the provided args
    bool HasAttachments(const Util::String& jointName, const Util::String& resName, Graphics::ModelEntity* graphicsEntity, Graphics::ModelEntity::Type graphicsEntityType=Graphics::ModelEntity::InvalidType) const;
    /// hide all matching attachments
    void HideAttachments(const Util::String& jointName, const Util::String& resName, Graphics::ModelEntity* graphicsENtity, Graphics::ModelEntity::Type graphicsEntityType=Graphics::ModelEntity::InvalidType) const;
    /// show all matching attachments
    void ShowAttachments(const Util::String& jointName, const Util::String& resName, Graphics::ModelEntity* graphicsENtity, Graphics::ModelEntity::Type graphicsEntityType=Graphics::ModelEntity::InvalidType) const;
    /// set fade / mix animations to attachments
    void FadeAnimation(FadeAnimation* msg);
    /// set base and overwrite animations to attachments
    void SetAnimation(SetAnimation* msg);

    /// get array with all currently attached graphics entities
    Util::Array<Ptr<Graphics::ModelEntity> > GetAttachmentGraphicsEntities() const;
    /// update attachment positions (call once per frame before the character is rendered)
    void UpdateAttachments();

    /// make skin visible on Character3
    bool AddSkin(const Util::String& skinName);
    /// make skin visible on Character3
    bool RemSkin(const Util::String& skinName);
    /// check if the skin exists for Character3
    bool HasSkin(const Util::String& skinName) const;

private:
    /// get array of attachment indices matching the provided search criteria
    Util::Array<IndexT> GetMatchingAttachmentIndices(const Util::String& jointName, const Util::String& resName, Graphics::ModelEntity* graphicsEntity, Graphics::ModelEntity::Type graphicsEntityType) const;

    /// an attachment
    struct Attachment
    {
        int jointIndex;
        Ptr<Graphics::ModelEntity> graphicsEntity;
        Math::matrix44 offsetMatrix;
        bool wasAttachedToLevel;
        bool firstFrame;
        bool isCharEntity;
    };

    Ptr<Graphics::ActorEntity> charGraphicsEntity;
    Util::Array<Attachment> attachments;
};

//------------------------------------------------------------------------------
/**
*/
inline void
CharEnhancementUtil::SetCharacterGraphicsEntity(Graphics::ActorEntity* entity)
{
    n_assert(0 != entity);
    this->charGraphicsEntity = entity;
}

//------------------------------------------------------------------------------
/**
*/
inline Graphics::ActorEntity*
CharEnhancementUtil::GetCharacterGraphicsEntity() const
{
    return this->charGraphicsEntity;
}

} // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif