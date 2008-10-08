//------------------------------------------------------------------------------
//  shellapplication.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/nsh3/shellapplication.h"

namespace Tools
{
using namespace Util;
using namespace IO;
using namespace Scripting;

//------------------------------------------------------------------------------
/**
*/
ShellApplication::ShellApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShellApplication::~ShellApplication()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
void
ShellApplication::Run()
{
    // first check for command line args
    if (this->args.HasArg("-help"))
    {
        n_printf("Nebula3 command shell.\n"
                 "Arguments:\n\n"
                 "-help: display this help\n"
                 "-file: evaluate script file and return\n");
    }
    else if (this->args.HasArg("-file"))
    {
        // FIXME!
    }
    else
    {
        Console* con = Console::Instance();
        ScriptServer* scriptServer = ScriptServer::Instance();
        con->Print("Welcome! Enter 'listcmds()' to get started or 'exit' to exit.\n");
        bool running = true;
        while (running)
        {
            con->Print("> ");
            while (!con->HasInput())
            {
                n_sleep(0.1);
            }
            String cmd = con->GetInput();
            if (cmd.IsValid())
            {
                if (cmd == "exit")
                {
                    running = false;
                }
                else
                {
                    if (!scriptServer->Eval(cmd))
                    {
                        con->Print("%s\n", scriptServer->GetError().AsCharPtr());
                    }
                }
            }
        }
    }
}

} // namespace Tools