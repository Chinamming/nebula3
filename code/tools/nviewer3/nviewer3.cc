//------------------------------------------------------------------------------
//  nviewer3.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/viewerapplication.h"

using namespace App;
using namespace Util;

ImplementNebulaApplication();

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CmdLineArgs& args)
{
    ViewerApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("nViewer3");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}
