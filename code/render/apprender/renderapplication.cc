//------------------------------------------------------------------------------
//  renderapplication.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/renderapplication.h"
#include "io/filestream.h"

#if !__WII__
#include "memory/debug/memorypagehandler.h"
#include "core/debug/corepagehandler.h"
#include "io/debug/iopagehandler.h"
#include "http/debug/svgtestpagehandler.h"
#else
#include "io/wii/wiidvdstream.h"
#endif

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
    #if __WII__        
        this->ioServer->RegisterUriScheme("file", IO::WiiDvdStream::RTTI);
        this->ioServer->RegisterUriScheme("dvd", IO::WiiDvdStream::RTTI);
    #else        
        this->ioServer->RegisterUriScheme("file", FileStream::RTTI);
    #endif  
        this->ioServer->SetAssign(Assign("shd", "home:export/shaders"));
        this->ioServer->SetAssign(Assign("frame", "home:export/frame"));
        this->ioServer->SetAssign(Assign("msh", "home:export/meshes"));
        this->ioServer->SetAssign(Assign("tex", "home:export/textures"));
        this->ioServer->SetAssign(Assign("ani", "home:export/anims"));
        this->ioServer->SetAssign(Assign("mdl", "home:export/gfxlib"));        

        // Nebul2 backward compat assigns:
        this->ioServer->SetAssign(Assign("meshes", "home:export/meshes"));
        this->ioServer->SetAssign(Assign("textures", "home:export/textures"));
        this->ioServer->SetAssign(Assign("anims", "home:export/anims"));

        this->ioInterface = IOInterface::Create();
        this->ioInterface->Open();

        // mount asset zip archives
        if (IoServer::Instance()->FileExists("home:export.zip"))
        {
            // main thread
            this->ioServer->MountZipArchive("home:export.zip");

            // io thread
            Ptr<Interface::MountZipArchive> mountZipArchiveMsg = Interface::MountZipArchive::Create();
            mountZipArchiveMsg->SetURI("home:export.zip");
            this->ioInterface->Send(mountZipArchiveMsg.upcast<Messaging::Message>());
        }

    #if !__WII__
        // setup http subsystem
        this->httpInterface = Http::HttpInterface::Create();
        this->httpInterface->Open();
        this->httpServerProxy = Http::HttpServerProxy::Create();
        this->httpServerProxy->Open();
        this->httpServerProxy->AttachRequestHandler(Debug::CorePageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::MemoryPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::IoPageHandler::Create());
        this->httpServerProxy->AttachRequestHandler(Debug::SvgTestPageHandler::Create());
        // setup remote control supsystem
        this->remoteInterface = Remote::RemoteInterface::Create();
        this->remoteInterface->Open();
        this->remoteControlProxy = Remote::RemoteControlProxy::Create();
        this->remoteControlProxy->Open();
    #endif

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

#if  !__WII__
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
    this->httpInterface->Close();
    this->httpInterface = 0;
    // remote control clean up
    this->remoteControlProxy->Close();
    this->remoteControlProxy = 0;
    this->remoteInterface->Close();
    this->remoteInterface = 0;
#endif    

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

    #if !__WII__
        // handle any http requests from the HttpServer thread
        this->httpServerProxy->HandlePendingRequests();
        // handle any remote requests from the RemoteControl thread
        this->remoteControlProxy->HandlePendingRequests();
    #endif
    
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