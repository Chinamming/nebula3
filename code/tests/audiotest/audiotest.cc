//------------------------------------------------------------------------------
//  audiotest.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audiotestapplication.h"

ImplementNebulaApplication();

using namespace Test;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CmdLineArgs& args)
{
    AudioTestApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("Audio Test");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }    
    app.Exit();
}