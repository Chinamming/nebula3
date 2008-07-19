//------------------------------------------------------------------------------
//  testclient.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testclient/testclientapplication.h"

using namespace Tools;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void __cdecl
main(int argc, const char** argv)
{
    CmdLineArgs args(argc, argv);
    TestClientApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("Test Client");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}
