#pragma once
#ifndef SYSTEM_APPENTRY_H
#define SYSTEM_APPENTRY_H
//------------------------------------------------------------------------------
/**
    @class System::AppEntry
    
    Implements a platform-indepent app-entry point. In your main file,
    put the line
    
    ImplementNebulaApplication();
    
    And then replace your main() function with:
    
    void NebulaMain(const CmdLineArgs& args)

    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
#if __WIN32__
#define ImplementNebulaApplication() \
void NebulaMain(const Util::CmdLineArgs& args); \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd) \
{ \
    Util::CmdLineArgs args(lpCmdLine); \
    NebulaMain(args); \
    return 0; \
}
#elif __XBOX360__
#define ImplementNebulaApplication() \
void NebulaMain(const Util::CmdLineArgs& args); \
void __cdecl main() \
{ \
    Util::CmdLineArgs args; \
    NebulaMain(args); \
}
#elif __WII__
#define ImplementNebulaApplication() \
void NebulaMain(const Util::CmdLineArgs& args); \
void main() \
{ \
    Util::CmdLineArgs args; \
    NebulaMain(args); \
}
#else
#define ImplementNebulaApplication() \
void NebulaMain(const Util::CmdLineArgs& args); \
int __cdecl main(int argc, const char** argv) \
{ \
    Util::CmdLineArgs args(argc, argv); \
    NebulaMain(args); \
    return 0; \
}
#endif
//------------------------------------------------------------------------------
#endif