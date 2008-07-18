//------------------------------------------------------------------------------
//  testasyncviewer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testasyncviewer/testasyncviewerapplication.h"

ImplementNebulaApplication();

using namespace Tools;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CmdLineArgs& args)
{
    TestAsyncViewerApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("Test Async Viewer");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}