//------------------------------------------------------------------------------
//  testviewer.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testgame/testgameapplication.h"

ImplementNebulaApplication()

using namespace Tools;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CmdLineArgs& args)
{
    TestGameApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("Test Game");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}