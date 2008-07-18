#pragma once
#ifndef ASYNCGRAPHICS_DISPLAYPROXY_H
#define ASYNCGRAPHICS_DISPLAYPROXY_H
//------------------------------------------------------------------------------
/**
    @class AsyncGraphics::DisplayProxy
    
    The DisplayProxy object is used to access DisplayDevice functionality
    from a different thread. Usually only the main thread creates a 
    display proxy object.

    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "coregraphics/adapter.h"
#include "coregraphics/displaymode.h"
#include "coregraphics/antialiasquality.h"
#include "coregraphics/pixelformat.h"
#include "coregraphics/adapterinfo.h"
#include "coregraphics/threadsafedisplayeventhandler.h"
#include "coregraphics/threadsaferendereventhandler.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
class DisplayProxy : public Core::RefCounted
{
    DeclareClass(DisplayProxy);
    DeclareSingleton(DisplayProxy);
public:
    /// constructor  
    DisplayProxy();
    /// destructor
    virtual ~DisplayProxy();

    /// set display adapter to use
    void SetAdapter(CoreGraphics::Adapter::Code a);
    /// get displat adapter
    CoreGraphics::Adapter::Code GetAdapter() const;
    /// set display mode (make sure the display mode is supported!)
    void SetDisplayMode(const CoreGraphics::DisplayMode& m);
    /// get display mode
    const CoreGraphics::DisplayMode& GetDisplayMode() const;
    /// set antialias quality
    void SetAntiAliasQuality(CoreGraphics::AntiAliasQuality::Code aa);
    /// get antialias quality
    CoreGraphics::AntiAliasQuality::Code GetAntiAliasQuality() const;
    /// set windowed/fullscreen mode
    void SetFullscreen(bool b);
    /// get windowed/fullscreen mode
    bool IsFullscreen() const;
    /// enable display mode switch when running fullscreen (default is true);
    void SetDisplayModeSwitchEnabled(bool b);
    /// is display mode switch enabled for fullscreen?
    bool IsDisplayModeSwitchEnabled() const;
    /// enable triple buffer for fullscreen (default is double buffering)
    void SetTripleBufferingEnabled(bool b);
    /// is triple buffer enabled for fullscreen?
    bool IsTripleBufferingEnabled() const;
    /// set always-on-top behaviour
    void SetAlwaysOnTop(bool b);
    /// get always-on-top behaviour
    bool IsAlwaysOnTop() const;
    /// turn vertical sync on/off
    void SetVerticalSyncEnabled(bool b);
    /// get vertical sync flag
    bool IsVerticalSyncEnabled() const;
    /// set optional window icon resource name
    void SetIconName(const Util::String& s);
    /// get optional window icon resource name
    const Util::String& GetIconName() const;

    /// open the display (waits for completion)
    void Open();
    /// close the display (waits for completion)
    void Close();
    /// return true if display is currently open
    bool IsOpen() const;

    /// return true if adapter exists (waits for completion)
    bool AdapterExists(CoreGraphics::Adapter::Code adapter);
    /// get available display modes on given adapter (waits for completion)
    Util::Array<CoreGraphics::DisplayMode> GetAvailableDisplayModes(CoreGraphics::Adapter::Code adapter, CoreGraphics::PixelFormat::Code pixelFormat);
    /// return true if a given display mode is supported (waits for completion)
    bool SupportsDisplayMode(CoreGraphics::Adapter::Code adapter, const CoreGraphics::DisplayMode& requestedMode);
    /// get current adapter display mode (i.e. the desktop display mode) (waits for completion)
    CoreGraphics::DisplayMode GetCurrentAdapterDisplayMode(CoreGraphics::Adapter::Code adapter);
    /// get general info about display adapter (waits for completion)
    CoreGraphics::AdapterInfo GetAdapterInfo(CoreGraphics::Adapter::Code adapter);
    /// attach a display event handler
    void AttachDisplayEventHandler(const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& displayEventHandler);
    /// remove a display event handler 
    void RemoveDisplayEventHandler(const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& displayEventHandler);
    /// attach a render event handler
    void AttachRenderEventHandler(const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& renderEventHandler);
    /// remove a render event handler
    void RemoveRenderEventHandler(const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& renderEventHandler);

private:
    CoreGraphics::Adapter::Code adapter;
    CoreGraphics::DisplayMode displayMode;
    CoreGraphics::AntiAliasQuality::Code antiAliasQuality;
    bool fullscreen;
    bool modeSwitchEnabled;
    bool tripleBufferingEnabled;
    bool alwaysOnTop;
    bool verticalSync;
    bool isOpen;
    Util::String iconName;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplayProxy::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayProxy::SetDisplayMode(const CoreGraphics::DisplayMode& m)
{
    this->displayMode = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const CoreGraphics::DisplayMode& 
DisplayProxy::GetDisplayMode() const
{
    return this->displayMode;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayProxy::SetAntiAliasQuality(CoreGraphics::AntiAliasQuality::Code aa)
{
    this->antiAliasQuality = aa;
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::AntiAliasQuality::Code
DisplayProxy::GetAntiAliasQuality() const
{
    return this->antiAliasQuality;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayProxy::SetAdapter(CoreGraphics::Adapter::Code a)
{
    this->adapter = a;
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::Adapter::Code
DisplayProxy::GetAdapter() const
{
    return this->adapter;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayProxy::SetFullscreen(bool b)
{
    this->fullscreen = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplayProxy::IsFullscreen() const
{
    return this->fullscreen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayProxy::SetDisplayModeSwitchEnabled(bool b)
{
    this->modeSwitchEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplayProxy::IsDisplayModeSwitchEnabled() const
{
    return this->modeSwitchEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayProxy::SetTripleBufferingEnabled(bool b)
{
    this->tripleBufferingEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplayProxy::IsTripleBufferingEnabled() const
{
    return this->tripleBufferingEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayProxy::SetAlwaysOnTop(bool b)
{
    this->alwaysOnTop = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplayProxy::IsAlwaysOnTop() const
{
    return this->alwaysOnTop;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayProxy::SetVerticalSyncEnabled(bool b)
{
    this->verticalSync = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplayProxy::IsVerticalSyncEnabled() const
{
    return this->verticalSync;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayProxy::SetIconName(const Util::String& s)
{
    this->iconName = s;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
DisplayProxy::GetIconName() const
{
    return this->iconName;
}

} // namespace AsyncGraphics
//------------------------------------------------------------------------------
#endif
    