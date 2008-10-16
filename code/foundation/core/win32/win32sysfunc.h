#pragma once
//------------------------------------------------------------------------------
/**
    @class Win32::SysFunc
    
    Provides Win32 specific helper functions.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win32
{

class SysFunc
{
public:
    /// setup lowlevel static objects (must be called before spawning any threads)
    static void Setup();
    /// exit process, and to proper cleanup, memleak reporting, etc...
    static void Exit(int exitCode);
    /// display an error message box
    static void Error(const char* error);
    /// print a message on the debug console
    static void DebugOut(const char* msg);
    /// sleep for a specified amount of seconds
    static void Sleep(double sec);

private:
    static bool volatile SetupCalled;
};

} // namespace Win32
//------------------------------------------------------------------------------
