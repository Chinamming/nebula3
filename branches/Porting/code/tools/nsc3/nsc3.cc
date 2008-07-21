//------------------------------------------------------------------------------
//  nsc3.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "shadercompilerapplication.h"

//------------------------------------------------------------------------------
/**
*/
int __cdecl
main(int argc, const char** argv)
{
    Util::CmdLineArgs args(argc, argv);
    Tools::ShaderCompilerApplication app;
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppName("Nebula3 Shader Compiler");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    int result = app.GetReturnCode();
    app.Exit();
    return result;
}

