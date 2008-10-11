#pragma once
#ifndef APP_RENDERAPPLICATION_H
#define APP_RENDERAPPLICATION_H
//------------------------------------------------------------------------------
/** 
    @class App::RenderApplication
    
    Render application class with multithreaded rendering.
    
    (C) 2008 Radon Labs GmbH
*/
#include "app/application.h"
#include "core/coreserver.h"
#include "debug/debuginterface.h"
#include "io/ioserver.h"
#include "interface/iointerface.h"
#include "graphics/graphicsinterface.h"
#include "graphics/display.h"
#include "input/inputserver.h"
#include "timing/timer.h"
#include "debug/debuginterface.h"
#include "debug/debugtimer.h"
#include "http/httpinterface.h"
#include "http/httpserverproxy.h"
#include "remote/remoteinterface.h"
#include "remote/remotecontrolproxy.h"

//------------------------------------------------------------------------------
namespace App
{
class RenderApplication : public Application
{
public:
    /// constructor
    RenderApplication();
    /// destructor
    virtual ~RenderApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();
    /// run the application
    virtual void Run();

private:
    /// update the current time values
    void UpdateTime();

protected:
    /// set quit requested flag
    void SetQuitRequested(bool b);
    /// return true if quit requested
    bool IsQuitRequested() const;
    /// called to configure display
    virtual void OnConfigureDisplay();
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// update world 
    virtual void OnUpdateFrame();
    /// get current absolute time
    Timing::Time GetTime() const;
    /// get current frame time
    Timing::Time GetFrameTime() const;

    Ptr<Core::CoreServer> coreServer;
    Ptr<Debug::DebugInterface> debugInterface;
    Ptr<IO::IoServer> ioServer;
    Ptr<Interface::IOInterface> ioInterface;
    Ptr<Graphics::GraphicsInterface> graphicsInterface;
    Ptr<Graphics::Display> display;    
    Ptr<Input::InputServer> inputServer;
    Ptr<Http::HttpInterface> httpInterface;
    Ptr<Http::HttpServerProxy> httpServerProxy;
    Ptr<Remote::RemoteInterface> remoteInterface;
    Ptr<Remote::RemoteControlProxy> remoteControlProxy;
    Timing::Timer timer;
    Timing::Time time;
    Timing::Time frameTime;
    bool quitRequested;

    _declare_timer(MainThreadFrameTimeAll);
    _declare_timer(MainThreadWaitForGraphicsFrame);
};

//------------------------------------------------------------------------------
/**
*/
inline void
RenderApplication::SetQuitRequested(bool b)
{
    this->quitRequested = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderApplication::IsQuitRequested() const
{
    return this->quitRequested;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
RenderApplication::GetTime() const
{
    return this->time;
}

//------------------------------------------------------------------------------
/**
*/
inline Timing::Time
RenderApplication::GetFrameTime() const
{
    return this->frameTime;
}

} // namespace App
//------------------------------------------------------------------------------
#endif
