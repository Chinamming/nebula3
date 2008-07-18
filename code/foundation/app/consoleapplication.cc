//------------------------------------------------------------------------------
//  consoleapplication.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "app/consoleapplication.h"
#include "io/filestream.h"
#include "io/memorystream.h"
#include "scripting/lua/luaserver.h"
#include "commands/iolibrary.h"
#include "commands/stdlibrary.h"

namespace App
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ConsoleApplication::ConsoleApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ConsoleApplication::~ConsoleApplication()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
ConsoleApplication::Open()
{
    n_assert(!this->IsOpen());
    if (Application::Open())
    {
        // initialize core subsystem
        this->coreServer = Core::CoreServer::Create();
        this->coreServer->SetCompanyName(this->companyName);
        this->coreServer->SetAppName(this->appName);
        this->coreServer->Open();

        // initialize io subsystem
        this->ioServer = IO::IoServer::Create();
        this->ioServer->RegisterUriScheme("file", IO::FileStream::RTTI);
        this->ioServer->RegisterUriScheme("mem", IO::MemoryStream::RTTI);

        // initialize scripting subsystem and register command libs
        this->scriptServer = Scripting::LuaServer::Create();
        this->scriptServer->Open();
        Commands::IOLibrary::Register();
        Commands::StdLibrary::Register();

        return true;   
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ConsoleApplication::Close()
{
    n_assert(this->IsOpen());

    // shutdown scripting subsystem
    this->scriptServer->Close();
    this->scriptServer = 0;

    // shutdown io subsystem
    this->ioServer = 0;

    // shutdown core subsystem
    this->coreServer = 0;

    Application::Close();
}

} // namespace App
