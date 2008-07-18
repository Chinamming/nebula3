//------------------------------------------------------------------------------
//  msg/gfxgetattachmententities.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/getattachmententities.h"
#include "graphics/modelentity.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::GetAttachmentEntities, 'GEAE', Messaging::Message);
ImplementMsgId(GetAttachmentEntities);

//------------------------------------------------------------------------------
/**
*/
GetAttachmentEntities::GetAttachmentEntities()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
GetAttachmentEntities::SetEntities(Util::Array<Ptr<Graphics::ModelEntity> > entities)
{
    this->entities = entities;
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<Ptr<Graphics::ModelEntity> >
GetAttachmentEntities::GetEntities()
{
    return this->entities;
}

}