#ifndef MSG_GFXGETATTACHMENTENTITIES_H
#define MSG_GFXGETATTACHMENTENTITIES_H
//------------------------------------------------------------------------------
/**
    @class Messaging::GetAttachmentEntities

    Access a character's attachment (gfx-)entities.
    
    (C) 2005 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "graphics/modelentity.h"

namespace Graphics
{
    class Entity;
}

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class GetAttachmentEntities : public Messaging::Message
{
    DeclareClass(GetAttachmentEntities);
    DeclareMsgId;

public:
    /// constructor
    GetAttachmentEntities();

    /// get attachment gfx entities
    Util::Array<Ptr<Graphics::ModelEntity> > GetEntities();
    /// set attachment gfx entities
    void SetEntities(Util::Array<Ptr<Graphics::ModelEntity> > entities);

private:
    Util::Array<Ptr<Graphics::ModelEntity> > entities;
};

//------------------------------------------------------------------------------
/**
*/

}; // namespace Msg
//------------------------------------------------------------------------------
#endif