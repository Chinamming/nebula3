//------------------------------------------------------------------------------
//  nsh3.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/nsh3/shellapplication.h"

using namespace Tools;
using namespace Util;

//------------------------------------------------------------------------------
/**
    NOTE: we cannot use ImplementNebulaApplication here, since nsh3 is 
    a command line tool.
*/
void __cdecl
main(int argc, const char** argv)
{
    CmdLineArgs args(argc, argv);
    ShellApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("Nebula3 Shell");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}
