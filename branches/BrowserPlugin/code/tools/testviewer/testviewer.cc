//------------------------------------------------------------------------------
//  testviewer.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testviewer/testviewerapplication.h"

ImplementNebulaApplication()

using namespace Tools;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CmdLineArgs& args)
{
    TestViewerApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("Test Viewer");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}