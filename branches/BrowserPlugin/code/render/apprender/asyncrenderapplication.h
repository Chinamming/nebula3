#pragma once
#ifndef APP_ASYNCRENDERAPPLICATION_H
#define APP_ASYNCRENDERAPPLICATION_H
//------------------------------------------------------------------------------
/** 
    @class App::AsyncRenderApplication
    
    Render application class with multithreaded rendering.
    
    (C) 2008 Radon Labs GmbH
*/
#include "app/application.h"
#include "core/coreserver.h"
#include "io/ioserver.h"
#include "interface/iointerface.h"
#include "asyncgraphics/asyncgraphicsinterface.h"
#include "asyncgraphics/displayproxy.h"
#include "input/inputserver.h"
#include "http/httpserver.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace App
{
class AsyncRenderApplication : public Application
{
public:
    /// constructor
    AsyncRenderApplication();
    /// destructor
    virtual ~AsyncRenderApplication();
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
    Ptr<IO::IoServer> ioServer;
    Ptr<Interface::IOInterface> ioInterface;
    Ptr<AsyncGraphics::AsyncGraphicsInterface> asyncGraphicsInterface;
    Ptr<AsyncGraphics::DisplayProxy> displayProxy;    
    Ptr<Input::InputServer> inputServer;
    Ptr<Http::HttpServer> httpServer;
    Timing::Timer timer;
    Timing::Time time;
    Timing::Time frameTime;
    bool quitRequested;
};

//------------------------------------------------------------------------------
/**
*/
inline void
AsyncRenderApplication::SetQuitRequested(bool b)
{
    this->quitRequested = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
AsyncRenderApplication::IsQuitRequested() const
{
    return this->quitRequested;
}

} // namespace App
//------------------------------------------------------------------------------
#endif
