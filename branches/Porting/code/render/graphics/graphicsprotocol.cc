//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsprotocol.h"

namespace Graphics
{
    __ImplementClass(Graphics::SetupGraphics, 'supg', Messaging::Message);
    ImplementMsgId(SetupGraphics);
    __ImplementClass(Graphics::AdapterExists, 'aext', Messaging::Message);
    ImplementMsgId(AdapterExists);
    __ImplementClass(Graphics::GetAvailableDisplayModes, 'gadm', Messaging::Message);
    ImplementMsgId(GetAvailableDisplayModes);
    __ImplementClass(Graphics::SupportsDisplayMode, 'spdm', Messaging::Message);
    ImplementMsgId(SupportsDisplayMode);
    __ImplementClass(Graphics::GetCurrentAdapterDisplayMode, 'gcdm', Messaging::Message);
    ImplementMsgId(GetCurrentAdapterDisplayMode);
    __ImplementClass(Graphics::GetAdapterInfo, 'gadi', Messaging::Message);
    ImplementMsgId(GetAdapterInfo);
    __ImplementClass(Graphics::AttachDisplayEventHandler, 'adeh', Messaging::Message);
    ImplementMsgId(AttachDisplayEventHandler);
    __ImplementClass(Graphics::RemoveDisplayEventHandler, 'rdeh', Messaging::Message);
    ImplementMsgId(RemoveDisplayEventHandler);
    __ImplementClass(Graphics::AttachRenderEventHandler, 'areh', Messaging::Message);
    ImplementMsgId(AttachRenderEventHandler);
    __ImplementClass(Graphics::RemoveRenderEventHandler, 'rreh', Messaging::Message);
    ImplementMsgId(RemoveRenderEventHandler);
    __ImplementClass(Graphics::CreateGraphicsStage, 'cgst', Messaging::Message);
    ImplementMsgId(CreateGraphicsStage);
    __ImplementClass(Graphics::DiscardGraphicsStage, 'digs', Messaging::Message);
    ImplementMsgId(DiscardGraphicsStage);
    __ImplementClass(Graphics::CreateGraphicsView, 'cgvw', Messaging::Message);
    ImplementMsgId(CreateGraphicsView);
    __ImplementClass(Graphics::DiscardGraphicsView, 'dgvw', Messaging::Message);
    ImplementMsgId(DiscardGraphicsView);
    __ImplementClass(Graphics::CreateGraphicsEntity, 'cget', Messaging::Message);
    ImplementMsgId(CreateGraphicsEntity);
    __ImplementClass(Graphics::DiscardGraphicsEntity, 'dcge', Messaging::Message);
    ImplementMsgId(DiscardGraphicsEntity);
    __ImplementClass(Graphics::CreateModelEntity, 'cmde', Messaging::Message);
    ImplementMsgId(CreateModelEntity);
    __ImplementClass(Graphics::CreateCameraEntity, 'cpce', Messaging::Message);
    ImplementMsgId(CreateCameraEntity);
    __ImplementClass(Graphics::CreateGlobalLightEntity, 'cgle', Messaging::Message);
    ImplementMsgId(CreateGlobalLightEntity);
    __ImplementClass(Graphics::CreateSpotLightEntity, 'csle', Messaging::Message);
    ImplementMsgId(CreateSpotLightEntity);
    __ImplementClass(Graphics::GraphicsEntityMsg, 'gemg', Messaging::Message);
    ImplementMsgId(GraphicsEntityMsg);
    __ImplementClass(Graphics::UpdateTransform, 'sttr', Messaging::Message);
    ImplementMsgId(UpdateTransform);
    __ImplementClass(Graphics::SetVisibility, 'svsb', Messaging::Message);
    ImplementMsgId(SetVisibility);
    __ImplementClass(Graphics::AttachCameraToView, 'actv', Messaging::Message);
    ImplementMsgId(AttachCameraToView);
    __ImplementClass(Graphics::RemoveCameraFromView, 'rcfv', Messaging::Message);
    ImplementMsgId(RemoveCameraFromView);
    __ImplementClass(Graphics::UpdateInstanceShaderVariable, 'uisv', Messaging::Message);
    ImplementMsgId(UpdateInstanceShaderVariable);
} // Graphics

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
