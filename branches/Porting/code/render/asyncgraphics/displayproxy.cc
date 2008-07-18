//------------------------------------------------------------------------------
//  displayproxy.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "asyncgraphics/displayproxy.h"
#include "asyncgraphics/protocol.h"
#include "asyncgraphics/asyncgraphicsinterface.h"

namespace AsyncGraphics
{
ImplementClass(AsyncGraphics::DisplayProxy, 'DSPX', Core::RefCounted);
ImplementSingleton(AsyncGraphics::DisplayProxy);

using namespace Util;
using namespace CoreGraphics;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
DisplayProxy::DisplayProxy() :
    displayMode(0, 0, 1024, 768, PixelFormat::X8R8G8B8),
    antiAliasQuality(AntiAliasQuality::None),
    adapter(Adapter::Primary),
    fullscreen(false),
    modeSwitchEnabled(true),
    tripleBufferingEnabled(false),
    alwaysOnTop(false),
    verticalSync(false),
    isOpen(false),
    iconName("Icon")
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DisplayProxy::~DisplayProxy()
{
    n_assert(!this->isOpen);
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
DisplayProxy::Open()
{
    n_assert(!this->isOpen);

    Ptr<AsyncGraphics::SetupGraphics> msg = AsyncGraphics::SetupGraphics::Create();
    msg->SetAdapter(this->adapter);
    msg->SetDisplayMode(this->displayMode);
    msg->SetAntiAliasQuality(this->antiAliasQuality);
    msg->SetFullscreen(this->fullscreen);
    msg->SetDisplayModeSwitchEnabled(this->modeSwitchEnabled);
    msg->SetTripleBufferingEnabled(this->tripleBufferingEnabled);
    msg->SetAlwaysOnTop(this->alwaysOnTop);
    msg->SetVerticalSyncEnabled(this->verticalSync);
    msg->SetIconName(this->iconName);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
DisplayProxy::Close()
{
    n_assert(this->isOpen);

    // FIXME: hmm, closing/reopening the display not currently supported
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
DisplayProxy::AdapterExists(CoreGraphics::Adapter::Code adapter)
{
    n_assert(this->isOpen);
    Ptr<AsyncGraphics::AdapterExists> msg = AsyncGraphics::AdapterExists::Create();
    msg->SetAdapter(adapter);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
Array<DisplayMode>
DisplayProxy::GetAvailableDisplayModes(Adapter::Code adapter, PixelFormat::Code pixelFormat)
{
    n_assert(this->isOpen);
    Ptr<AsyncGraphics::GetAvailableDisplayModes> msg = AsyncGraphics::GetAvailableDisplayModes::Create();
    msg->SetAdapter(adapter);
    msg->SetPixelFormat(pixelFormat);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
bool
DisplayProxy::SupportsDisplayMode(Adapter::Code adapter, const DisplayMode& requestedMode)
{
    n_assert(this->isOpen);
    Ptr<AsyncGraphics::SupportsDisplayMode> msg = AsyncGraphics::SupportsDisplayMode::Create();
    msg->SetAdapter(adapter);
    msg->SetDisplayMode(requestedMode);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
DisplayMode
DisplayProxy::GetCurrentAdapterDisplayMode(Adapter::Code adapter)
{
    n_assert(this->isOpen);
    Ptr<AsyncGraphics::GetCurrentAdapterDisplayMode> msg = AsyncGraphics::GetCurrentAdapterDisplayMode::Create();
    msg->SetAdapter(adapter);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
AdapterInfo
DisplayProxy::GetAdapterInfo(Adapter::Code adapter)
{
    n_assert(this->isOpen);
    Ptr<AsyncGraphics::GetAdapterInfo> msg = AsyncGraphics::GetAdapterInfo::Create();
    msg->SetAdapter(adapter);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    return msg->GetResult();
}

//------------------------------------------------------------------------------
/**
*/
void
DisplayProxy::AttachDisplayEventHandler(const Ptr<ThreadSafeDisplayEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<AsyncGraphics::AttachDisplayEventHandler> msg = AsyncGraphics::AttachDisplayEventHandler::Create();
    msg->SetHandler(handler);
    AsyncGraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
DisplayProxy::RemoveDisplayEventHandler(const Ptr<ThreadSafeDisplayEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<AsyncGraphics::RemoveDisplayEventHandler> msg = AsyncGraphics::RemoveDisplayEventHandler::Create();
    msg->SetHandler(handler);
    AsyncGraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
DisplayProxy::AttachRenderEventHandler(const Ptr<ThreadSafeRenderEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<AsyncGraphics::AttachRenderEventHandler> msg = AsyncGraphics::AttachRenderEventHandler::Create();
    msg->SetHandler(handler);
    AsyncGraphicsInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
DisplayProxy::RemoveRenderEventHandler(const Ptr<ThreadSafeRenderEventHandler>& handler)
{
    n_assert(this->isOpen);
    Ptr<AsyncGraphics::RemoveRenderEventHandler> msg = AsyncGraphics::RemoveRenderEventHandler::Create();
    msg->SetHandler(handler);
    AsyncGraphicsInterface::Instance()->Send(msg.cast<Message>());
}

} // namespace AsyncGraphics
