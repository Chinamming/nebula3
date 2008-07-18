//------------------------------------------------------------------------------
//  game/corefeature.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corefeature/corefeatureunit.h"
#include "app/application.h"
#include "io/filestream.h"
#include "memory/debug/memorypagehandler.h"

#if __USE_SCRIPTING__
#include "scripting/lua/luaserver.h"
#include "commands/iolibrary.h"
#include "commands/stdlibrary.h"
#endif

#if __USE_HTTP__
#include "io/debug/iopagehandler.h"
#include "core/debug/corepagehandler.h"
#endif

#if __USE_HTTP__ && __USE_SCRIPTING__
#include "scripting/debug/scriptingpagehandler.h"
#endif

#if __WII__
#include "io/wii/wiidvdstream.h"
#endif

namespace CoreFeature
{
ImplementClass(CoreFeatureUnit, 'COFU' , Game::FeatureUnit);
ImplementSingleton(CoreFeatureUnit);

using namespace App;
using namespace Core;
using namespace Util;
using namespace Math;
using namespace IO;
using namespace Interface;

//------------------------------------------------------------------------------
/**
*/
CoreFeatureUnit::CoreFeatureUnit()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
CoreFeatureUnit::~CoreFeatureUnit()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
CoreFeatureUnit::OnActivate()
{
	FeatureUnit::OnActivate();
    
    // setup core subsystem
    this->coreServer = CoreServer::Create();
    this->coreServer->SetCompanyName(Application::Instance()->GetCompanyName());
    this->coreServer->SetAppName(Application::Instance()->GetAppName());
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
    this->ioServer->SetAssign(Assign("data", "home:export/data"));        
    this->ioServer->SetAssign(Assign("export", "home:export"));

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

#if __USE_HTTP__
    // setup debug http server
    this->httpServer = Http::HttpServer::Create();
    this->httpServer->Open();
    this->httpServer->AttachRequestHandler(Debug::MemoryPageHandler::Create());
    this->httpServer->AttachRequestHandler(Debug::CorePageHandler::Create());
    this->httpServer->AttachRequestHandler(Debug::IoPageHandler::Create());
    this->httpServer->AttachRequestHandler(Debug::ScriptingPageHandler::Create());
#endif    

#if __USE_SCRIPTING__    
    // setup scripting subsystem
    this->scriptServer = Scripting::LuaServer::Create();
    this->scriptServer->Open();
    Commands::IOLibrary::Register();
    Commands::StdLibrary::Register();
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
CoreFeatureUnit::OnDeactivate()
{    
#if __USE_SCRIPTING__ 
    this->scriptServer->Close();
    this->scriptServer = 0;
#endif

#if __USE_HTTP__
    this->httpServer->Close();
    this->httpServer = 0;
#endif
        
    this->ioInterface->Close();
    this->ioInterface = 0;
    this->ioServer = 0;

    this->coreServer->Close();
    this->coreServer = 0;

    FeatureUnit::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
CoreFeatureUnit::OnBeginFrame()
{  
#if __USE_HTTP__
    this->httpServer->OnFrame(); 
#endif
}

}; // namespace Game
