//------------------------------------------------------------------------------
//  renderapplication.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/renderapplication.h"
#include "io/filestream.h"
#include "memory/debug/memorypagehandler.h"
#include "core/debug/corepagehandler.h"
#include "io/debug/iopagehandler.h"
#include "http/debug/svgtestpagehandler.h"
#include "threading/debug/threadpagehandler.h"

namespace App
{
using namespace Core;
using namespace Debug;
using namespace IO;
using namespace Interface;
using namespace Graphics;
using namespace Input;
using namespace CoreGraphics;
using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
RenderApplication::RenderApplication() :
    time(0.0),
    frameTime(0.0),
    quitRequested(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RenderApplication::~RenderApplication()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
RenderApplication::Open()
{
    n_assert(!this->IsOpen());
    if (Application::Open())
    {
        // setup core subsystem
        this->coreServer = CoreServer::Create();
        this->coreServer->SetCompanyName(this->companyName);
        this->coreServer->SetAppName(this->appName);
        this->coreServer->Open();

        // setup io subsystem
        // FIXME: REDUNDANT!!! 
        this->ioServer = IoServer::Create();
        this->ioServer->RegisterStandardUriSchemes();
        this->ioServer->SetupStandardAssigns();
        this->ioServer->MountStandardZipArchives();

        this->ioInterface = IOInterface::Create();
        this->ioInterface->Open();

        // setup http subsystem
        this->httpInterface = Http::HttpInterface::Create();
        this->httpInterface->Open();
        this->httpServerProxy = Http::HttpServerProxy::Create();
        this->httpServerProxy->Open();
        this->httpServerProxy->AttachRequestHandler(Debug::CorePageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::ThreadPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::MemoryPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::IoPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::SvgTestPageHandler::Create());

        // setup remote subsystem
        this->remoteInterface = Remote::RemoteInterface::Create();
        this->remoteInterface->Open();
        this->remoteControlProxy = Remote::RemoteControlProxy::Create();
        this->remoteControlProxy->Open();

        // setup debug subsystem
        this->debugInterface = DebugInterface::Create();
        this->debugInterface->Open();

        // setup asynchronous graphics subsystem
        this->graphicsInterface = GraphicsInterface::Create();
        this->graphicsInterface->Open();
        this->display = Display::Create();
        this->OnConfigureDisplay();
        this->display->Open();

        // setup input subsystem
        this->inputServer = InputServer::Create();
        this->inputServer->Open();

        // setup frame timer
        this->timer.Start();
        this->time = 0.0;
        this->frameTime = 0.01;

        // setup debug timers and counters
        _setup_timer(MainThreadFrameTimeAll);
        _setup_timer(MainThreadWaitForGraphicsFrame);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnConfigureDisplay()
{
    // display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->display->AdapterExists(adapter))
        {
            this->display->SetAdapter(adapter);
        }
    }

    // display mode
    DisplayMode displayMode;
    if (this->args.HasArg("-x"))
    {
        displayMode.SetXPos(this->args.GetInt("-x"));
    }
    if (this->args.HasArg("-y"))
    {
        displayMode.SetYPos(this->args.GetInt("-y"));
    }
    if (this->args.HasArg("-w"))
    {
        displayMode.SetWidth(this->args.GetInt("-w"));
    }
    if (this->args.HasArg("-h"))
    {
        displayMode.SetHeight(this->args.GetInt("-h"));
    }
    this->display->SetDisplayMode(displayMode);
    this->display->SetFullscreen(this->args.GetBool("-fullscreen"));
    this->display->SetAlwaysOnTop(this->args.GetBool("-alwaysontop"));
    this->display->SetVerticalSyncEnabled(this->args.GetBool("-vsync"));
    if (this->args.HasArg("-aa"))
    {
        this->display->SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }
    #if __XBOX360__
        this->display->SetAntiAliasQuality(AntiAliasQuality::Medium);
    #endif
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Close()
{
    n_assert(this->IsOpen());
    
    _discard_timer(MainThreadFrameTimeAll);
    _discard_timer(MainThreadWaitForGraphicsFrame);

    this->timer.Stop();

    this->inputServer->Close();
    this->inputServer = 0;

    this->display->Close();
    this->display = 0;

    this->graphicsInterface->Close();
    this->graphicsInterface = 0;

    this->debugInterface->Close();
    this->debugInterface = 0;

    this->remoteControlProxy->Close();
    this->remoteControlProxy = 0;
    this->remoteInterface->Close();
    this->remoteInterface = 0;

    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
    this->httpInterface->Close();
    this->httpInterface = 0;

    this->ioInterface->Close();
    this->ioInterface = 0;
    this->ioServer = 0;

    this->coreServer->Close();
    this->coreServer = 0;

    Application::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Run()
{
    n_assert(this->isOpen);
    while (!(this->inputServer->IsQuitRequested() || this->IsQuitRequested()))
    {
        _start_timer(MainThreadFrameTimeAll);

        // synchronize with the graphics frame, to prevent the game thread
        // to run ahead of the graphics thread
        _start_timer(MainThreadWaitForGraphicsFrame);
        GraphicsInterface::Instance()->WaitForFrameEvent();
        _stop_timer(MainThreadWaitForGraphicsFrame);

        // handle any http requests from the HttpServer thread
        this->httpServerProxy->HandlePendingRequests();

        // handle any remote requests from the RemoteControl thread
        this->remoteControlProxy->HandlePendingRequests();
    
        // process input
        this->inputServer->BeginFrame();
        this->inputServer->OnFrame();
        this->OnProcessInput();

        // update time
        this->UpdateTime();

        // run "game logic"
        this->OnUpdateFrame();

        this->inputServer->EndFrame();

        _stop_timer(MainThreadFrameTimeAll);

        if (!this->display->IsFullscreen())
        {
            Core::SysFunc::Sleep(0.0);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnProcessInput()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnUpdateFrame()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::UpdateTime()
{
    Time curTime = this->timer.GetTime();
    this->frameTime = curTime - this->time;
    this->time = curTime;
}

} // namespace App
