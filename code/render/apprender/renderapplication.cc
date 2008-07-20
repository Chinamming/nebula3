//------------------------------------------------------------------------------
//  renderapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/renderapplication.h"
#include "io/filestream.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/texture.h"
#include "coregraphics/mesh.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "resources/managedtexture.h"
#include "resources/managedmesh.h"
#include "frame/frameserver.h"
#include "coregraphics/streamanimationloader.h"
#include "nebula2/anim/managedanimation.h"
#include "addons/nebula2/nebula2wrapper.h"

#if !__WII__
#include "scripting/lua/luaserver.h"
#include "commands/iolibrary.h"
#include "commands/stdlibrary.h"
#include "scripting/debug/scriptingpagehandler.h"
#include "io/debug/iopagehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "core/debug/corepagehandler.h"
#include "coregraphics/debug/displaypagehandler.h"
#include "coregraphics/debug/texturepagehandler.h"
#include "coregraphics/debug/meshpagehandler.h"
#include "coregraphics/debug/shaderpagehandler.h"
#else
#include "io/wii/wiidvdstream.h"
#endif

namespace App
{
using namespace Core;
using namespace Util;
using namespace Math;
using namespace IO;
using namespace Interface;
using namespace CoreGraphics;
using namespace Resources;
using namespace Models;
using namespace Graphics;
using namespace Lighting;
using namespace Input;
using namespace Frame;
using namespace Timing;
using namespace Anim;

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
        // setup debug http server
        this->httpServer = Http::HttpServer::Create();
        this->httpServer->Open();
        this->httpServer->AttachRequestHandler(Debug::MemoryPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::CorePageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::IoPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::ScriptingPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::DisplayPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::TexturePageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::MeshPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::ShaderPageHandler::Create());
        
        // setup scripting subsystem
        this->scriptServer = Scripting::LuaServer::Create();
        this->scriptServer->Open();
        Commands::IOLibrary::Register();
        Commands::StdLibrary::Register();    
#endif
        // setup core graphics subsystem
        this->displayDevice = DisplayDevice::Create();
        this->OnConfigureDisplayDevice();
        if (!this->displayDevice->Open())
        {
            n_error("RenderApplication: Failed to open display device!");
            return false;
        }
        this->renderDevice = RenderDevice::Create();
        if (!this->renderDevice->Open())
        {
            n_error("RenderApplication: Failed to open render device!");
            return false;
        }
        this->vertexLayoutServer = VertexLayoutServer::Create();
        this->vertexLayoutServer->Open();
        this->shaderServer = ShaderServer::Create();
        this->shaderServer->Open();
        this->transformDevice = TransformDevice::Create();
        this->transformDevice->Open();
        this->shapeRenderer = ShapeRenderer::Create();
        this->shapeRenderer->Open();

        // setup resource subsystem
        this->sharedResourceServer = SharedResourceServer::Create();
        this->sharedResourceServer->Open();
        this->resourceManager = ResourceManager::Create();
        this->resourceManager->Open();

        // setup frame server
        this->frameServer = FrameServer::Create();
        this->frameServer->Open();

        // setup resource mapper for textures
        Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
        texMapper->SetPlaceholderResourceId(ResourceId(PLACEHOLDER_TEXTURENAME));
        texMapper->SetResourceClass(Texture::RTTI);
        texMapper->SetResourceLoaderClass(StreamTextureLoader::RTTI);
        texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
        this->resourceManager->AttachMapper(texMapper.upcast<ResourceMapper>());

        // setup resource mapper for meshes
        Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
        meshMapper->SetPlaceholderResourceId(ResourceId("msh:system/placeholder_s_0.nvx2"));
        meshMapper->SetResourceClass(Mesh::RTTI);
        meshMapper->SetResourceLoaderClass(StreamMeshLoader::RTTI);
        meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
        this->resourceManager->AttachMapper(meshMapper.upcast<ResourceMapper>());

        // setup resource mapper for animations
        Ptr<SimpleResourceMapper> animMapper = SimpleResourceMapper::Create();
        animMapper->SetPlaceholderResourceId(ResourceId("ani:examples/eagle.nax2"));
        animMapper->SetResourceClass(MemoryAnimation::RTTI);
        animMapper->SetResourceLoaderClass(StreamAnimationLoader::RTTI);
        animMapper->SetManagedResourceClass(ManagedAnimation::RTTI);
        this->resourceManager->AttachMapper(animMapper.upcast<ResourceMapper>());

        // setup input subsystem
        this->inputServer = InputServer::Create();
        this->inputServer->SetParentHwnd(this->displayDevice->GetParentHwnd());
        this->inputServer->Open();
        
        // setup model server
        this->modelServer = ModelServer::Create();
        this->modelServer->Open();

        // setup graphics subsystem
        this->graphicsServer = GraphicsServer::Create();
        this->graphicsServer->Open();

        // setup lighting subsystem
        this->lightServer = LightServer::Create();
        this->lightServer->Open();
        this->shadowServer = ShadowServer::Create();
        this->shadowServer->Open();

        this->animationServer = Anim::AnimationServer::Create();

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
RenderApplication::OnConfigureDisplayDevice()
{
    // display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->displayDevice->AdapterExists(adapter))
        {
            this->displayDevice->SetAdapter(adapter);
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
    this->displayDevice->SetDisplayMode(displayMode);
    this->displayDevice->SetFullscreen(this->args.GetBool("-fullscreen"));
    this->displayDevice->SetAlwaysOnTop(this->args.GetBool("-alwaysontop"));
    this->displayDevice->SetVerticalSyncEnabled(this->args.GetBool("-vsync"));
    if (this->args.HasArg("-aa"))
    {
        this->displayDevice->SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }

    // window title
    if (this->args.HasArg("-windowtitle"))
    {
        this->displayDevice->SetWindowTitle(this->args.GetString("-windowtitle"));
    }
    else
    {
        this->displayDevice->SetWindowTitle("Nebula3 Viewer");
    }

#if __WIN32__
#if _DEBUG
    Timing::Sleep(10); // you've got 10 secs to hook the debugger up, go go go!
#endif // _DEBUG
    // run the app in a child window of the given parent window
    if (this->args.HasArg("-parentwnd"))
    {
        String wndStr = this->args.GetString("-parentwnd");
        HWND hWndParent = (HWND)_atoi64(wndStr.AsCharPtr());
        this->displayDevice->SetParentHwnd(hWndParent);
    }
#endif //__WIN32__
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Close()
{
    n_assert(this->IsOpen());

    this->shadowServer->Close();
    this->shadowServer = 0;

    this->lightServer->Close();
    this->lightServer = 0;

    this->graphicsServer->Close();
    this->graphicsServer = 0;

    this->modelServer->Close();
    this->modelServer = 0;

    this->resourceManager->Close();
    this->resourceManager = 0;

    this->inputServer->Close();
    this->inputServer = 0;

    this->frameServer->Close();
    this->frameServer = 0;

    this->sharedResourceServer->Close();
    this->sharedResourceServer = 0;

    this->shapeRenderer->Close();
    this->shapeRenderer = 0;

    this->shaderServer->Close();
    this->shaderServer = 0;

    this->transformDevice->Close();
    this->transformDevice = 0;

    this->vertexLayoutServer->Close();
    this->vertexLayoutServer = 0;

    this->renderDevice->Close();
    this->renderDevice = 0;

    this->displayDevice->Close();
    this->displayDevice = 0;
    
#if !__WII__
    this->scriptServer->Close();
    this->scriptServer = 0;
    
    this->httpServer->Close();
    this->httpServer = 0;
#endif        
    this->ioInterface->Close();
    this->ioInterface = 0;
    this->ioServer = 0;

    this->coreServer->Close();
    this->coreServer = 0;
    
    this->animationServer = 0;

    Application::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Run()
{
    n_assert(this->IsOpen());
    while (!(this->inputServer->IsQuitRequested() || this->IsQuitRequested()))
    {
#if !__WII__    
        this->httpServer->OnFrame();
#endif        
        this->inputServer->BeginFrame();
        this->inputServer->OnFrame();
        this->OnProcessInput();
        this->UpdateTime();
        this->OnUpdateFrame();
        this->OnRenderFrame();
        this->inputServer->EndFrame();

        // if we're running in windowed mode, give up time slice, 
        // for better responsives of other apps
        if (!this->displayDevice->IsFullscreen())
        {
            Timing::Sleep(0.0f);
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
RenderApplication::OnRenderFrame()
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