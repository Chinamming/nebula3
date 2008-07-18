//------------------------------------------------------------------------------
//  asyncrenderapplication.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/asyncrenderapplication.h"
#include "memory/debug/memorypagehandler.h"
#include "core/debug/corepagehandler.h"
#include "io/debug/iopagehandler.h"
#include "io/filestream.h"

namespace App
{
using namespace Core;
using namespace IO;
using namespace Interface;
using namespace AsyncGraphics;
using namespace Http;
using namespace Input;
using namespace CoreGraphics;
using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
AsyncRenderApplication::AsyncRenderApplication() :
    time(0.0),
    frameTime(0.0),
    quitRequested(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AsyncRenderApplication::~AsyncRenderApplication()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
AsyncRenderApplication::Open()
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
        this->ioServer->RegisterUriScheme("file", FileStream::RTTI);
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

        // setup debug http server
        this->httpServer = HttpServer::Create();
        this->httpServer->Open();
        this->httpServer->AttachRequestHandler(Debug::MemoryPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::CorePageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::IoPageHandler::Create());

        // setup asynchronous graphics subsystem
        this->asyncGraphicsInterface = AsyncGraphicsInterface::Create();
        this->asyncGraphicsInterface->Open();
        this->displayProxy = DisplayProxy::Create();
        this->OnConfigureDisplay();
        this->displayProxy->Open();

        // setup input subsystem
        this->inputServer = InputServer::Create();
        this->inputServer->Open();

        // setup frame timer
        this->timer.Start();
        this->time = 0.0;
        this->frameTime = 0.01;

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncRenderApplication::OnConfigureDisplay()
{
    // display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->displayProxy->AdapterExists(adapter))
        {
            this->displayProxy->SetAdapter(adapter);
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
    this->displayProxy->SetDisplayMode(displayMode);
    this->displayProxy->SetFullscreen(this->args.GetBool("-fullscreen"));
    this->displayProxy->SetAlwaysOnTop(this->args.GetBool("-alwaysontop"));
    this->displayProxy->SetVerticalSyncEnabled(this->args.GetBool("-vsync"));
    if (this->args.HasArg("-aa"))
    {
        this->displayProxy->SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncRenderApplication::Close()
{
    n_assert(this->IsOpen());
    
    this->inputServer->Close();
    this->inputServer = 0;

    this->displayProxy->Close();
    this->displayProxy = 0;

    this->asyncGraphicsInterface->Close();
    this->asyncGraphicsInterface = 0;

    this->httpServer->Close();
    this->httpServer = 0;

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
AsyncRenderApplication::Run()
{
    n_assert(this->isOpen);
    while (!(this->inputServer->IsQuitRequested() || this->IsQuitRequested()))
    {
        this->httpServer->OnFrame();
        this->inputServer->BeginFrame();
        this->inputServer->OnFrame();
        this->OnProcessInput();
        this->UpdateTime();
        this->OnUpdateFrame();
        this->inputServer->EndFrame();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncRenderApplication::OnProcessInput()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncRenderApplication::OnUpdateFrame()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncRenderApplication::UpdateTime()
{
    Time curTime = this->timer.GetTime();
    this->frameTime = curTime - this->time;
    this->time = curTime;
}

} // namespace App
