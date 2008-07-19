//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "protocol.h"

namespace AsyncGraphics
{
    ImplementClass(AsyncGraphics::SetupGraphics, 'supg', Messaging::Message);
    ImplementMsgId(SetupGraphics);
    ImplementClass(AsyncGraphics::AdapterExists, 'aext', Messaging::Message);
    ImplementMsgId(AdapterExists);
    ImplementClass(AsyncGraphics::GetAvailableDisplayModes, 'gadm', Messaging::Message);
    ImplementMsgId(GetAvailableDisplayModes);
    ImplementClass(AsyncGraphics::SupportsDisplayMode, 'spdm', Messaging::Message);
    ImplementMsgId(SupportsDisplayMode);
    ImplementClass(AsyncGraphics::GetCurrentAdapterDisplayMode, 'gcdm', Messaging::Message);
    ImplementMsgId(GetCurrentAdapterDisplayMode);
    ImplementClass(AsyncGraphics::GetAdapterInfo, 'gadi', Messaging::Message);
    ImplementMsgId(GetAdapterInfo);
    ImplementClass(AsyncGraphics::AttachDisplayEventHandler, 'adeh', Messaging::Message);
    ImplementMsgId(AttachDisplayEventHandler);
    ImplementClass(AsyncGraphics::RemoveDisplayEventHandler, 'rdeh', Messaging::Message);
    ImplementMsgId(RemoveDisplayEventHandler);
    ImplementClass(AsyncGraphics::AttachRenderEventHandler, 'areh', Messaging::Message);
    ImplementMsgId(AttachRenderEventHandler);
    ImplementClass(AsyncGraphics::RemoveRenderEventHandler, 'rreh', Messaging::Message);
    ImplementMsgId(RemoveRenderEventHandler);
    ImplementClass(AsyncGraphics::CreateGraphicsStage, 'cgst', Messaging::Message);
    ImplementMsgId(CreateGraphicsStage);
    ImplementClass(AsyncGraphics::DiscardGraphicsStage, 'digs', Messaging::Message);
    ImplementMsgId(DiscardGraphicsStage);
    ImplementClass(AsyncGraphics::CreateGraphicsView, 'cgvw', Messaging::Message);
    ImplementMsgId(CreateGraphicsView);
    ImplementClass(AsyncGraphics::DiscardGraphicsView, 'dgvw', Messaging::Message);
    ImplementMsgId(DiscardGraphicsView);
    ImplementClass(AsyncGraphics::CreateGraphicsEntity, 'cget', Messaging::Message);
    ImplementMsgId(CreateGraphicsEntity);
    ImplementClass(AsyncGraphics::DiscardGraphicsEntity, 'dcge', Messaging::Message);
    ImplementMsgId(DiscardGraphicsEntity);
    ImplementClass(AsyncGraphics::CreateModelEntity, 'cmde', Messaging::Message);
    ImplementMsgId(CreateModelEntity);
    ImplementClass(AsyncGraphics::CreateCameraEntity, 'cpce', Messaging::Message);
    ImplementMsgId(CreateCameraEntity);
    ImplementClass(AsyncGraphics::CreateGlobalLightEntity, 'cgle', Messaging::Message);
    ImplementMsgId(CreateGlobalLightEntity);
    ImplementClass(AsyncGraphics::CreateSpotLightEntity, 'csle', Messaging::Message);
    ImplementMsgId(CreateSpotLightEntity);
    ImplementClass(AsyncGraphics::GraphicsEntityMsg, 'gemg', Messaging::Message);
    ImplementMsgId(GraphicsEntityMsg);
    ImplementClass(AsyncGraphics::UpdateTransform, 'sttr', Messaging::Message);
    ImplementMsgId(UpdateTransform);
    ImplementClass(AsyncGraphics::SetVisibility, 'svsb', Messaging::Message);
    ImplementMsgId(SetVisibility);
} // AsyncGraphics

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
