//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsprotocol.h"

namespace Graphics
{
    ImplementClass(Graphics::SetupGraphics, 'supg', Messaging::Message);
    ImplementMsgId(SetupGraphics);
    ImplementClass(Graphics::AdapterExists, 'aext', Messaging::Message);
    ImplementMsgId(AdapterExists);
    ImplementClass(Graphics::GetAvailableDisplayModes, 'gadm', Messaging::Message);
    ImplementMsgId(GetAvailableDisplayModes);
    ImplementClass(Graphics::SupportsDisplayMode, 'spdm', Messaging::Message);
    ImplementMsgId(SupportsDisplayMode);
    ImplementClass(Graphics::GetCurrentAdapterDisplayMode, 'gcdm', Messaging::Message);
    ImplementMsgId(GetCurrentAdapterDisplayMode);
    ImplementClass(Graphics::GetAdapterInfo, 'gadi', Messaging::Message);
    ImplementMsgId(GetAdapterInfo);
    ImplementClass(Graphics::AttachDisplayEventHandler, 'adeh', Messaging::Message);
    ImplementMsgId(AttachDisplayEventHandler);
    ImplementClass(Graphics::RemoveDisplayEventHandler, 'rdeh', Messaging::Message);
    ImplementMsgId(RemoveDisplayEventHandler);
    ImplementClass(Graphics::AttachRenderEventHandler, 'areh', Messaging::Message);
    ImplementMsgId(AttachRenderEventHandler);
    ImplementClass(Graphics::RemoveRenderEventHandler, 'rreh', Messaging::Message);
    ImplementMsgId(RemoveRenderEventHandler);
    ImplementClass(Graphics::CreateGraphicsStage, 'cgst', Messaging::Message);
    ImplementMsgId(CreateGraphicsStage);
    ImplementClass(Graphics::DiscardGraphicsStage, 'digs', Messaging::Message);
    ImplementMsgId(DiscardGraphicsStage);
    ImplementClass(Graphics::CreateGraphicsView, 'cgvw', Messaging::Message);
    ImplementMsgId(CreateGraphicsView);
    ImplementClass(Graphics::DiscardGraphicsView, 'dgvw', Messaging::Message);
    ImplementMsgId(DiscardGraphicsView);
    ImplementClass(Graphics::CreateGraphicsEntity, 'cget', Messaging::Message);
    ImplementMsgId(CreateGraphicsEntity);
    ImplementClass(Graphics::DiscardGraphicsEntity, 'dcge', Messaging::Message);
    ImplementMsgId(DiscardGraphicsEntity);
    ImplementClass(Graphics::CreateModelEntity, 'cmde', Messaging::Message);
    ImplementMsgId(CreateModelEntity);
    ImplementClass(Graphics::CreateCameraEntity, 'cpce', Messaging::Message);
    ImplementMsgId(CreateCameraEntity);
    ImplementClass(Graphics::CreateGlobalLightEntity, 'cgle', Messaging::Message);
    ImplementMsgId(CreateGlobalLightEntity);
    ImplementClass(Graphics::CreateSpotLightEntity, 'csle', Messaging::Message);
    ImplementMsgId(CreateSpotLightEntity);
    ImplementClass(Graphics::GraphicsEntityMsg, 'gemg', Messaging::Message);
    ImplementMsgId(GraphicsEntityMsg);
    ImplementClass(Graphics::UpdateTransform, 'sttr', Messaging::Message);
    ImplementMsgId(UpdateTransform);
    ImplementClass(Graphics::SetVisibility, 'svsb', Messaging::Message);
    ImplementMsgId(SetVisibility);
    ImplementClass(Graphics::AttachCameraToView, 'actv', Messaging::Message);
    ImplementMsgId(AttachCameraToView);
    ImplementClass(Graphics::RemoveCameraFromView, 'rcfv', Messaging::Message);
    ImplementMsgId(RemoveCameraFromView);
    ImplementClass(Graphics::RenderDebugText, 'rdtx', Messaging::Message);
    ImplementMsgId(RenderDebugText);
} // Graphics

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
