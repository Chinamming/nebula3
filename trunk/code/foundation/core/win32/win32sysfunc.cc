//------------------------------------------------------------------------------
//  sysfunc.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/win32/win32sysfunc.h"
#include "core/refcounted.h"
#include "debug/minidump.h"
#include "util/blob.h"
#include "util/guid.h"

namespace Win32
{
using namespace Util;

bool volatile SysFunc::SetupCalled = false;

//------------------------------------------------------------------------------
/**
    This method must be called at application start before any threads
    are spawned. It is used to setup static objects beforehand (i.e.
    private heaps of various utility classes). Doing this eliminates
    the need for fine-grained locking in the affected classes.
*/
void
SysFunc::Setup()
{
    if (!SetupCalled)
    {
        SetupCalled = true;
        Memory::Heap::Setup();
        Blob::Setup();
        String::Setup();
        Guid::Setup();
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by Application::Exit(), or otherwise must be
    called right before the end of the programs main() function. The method
    will properly shutdown the Nebula3 runtime environment, and report 
    refcounting and memory leaks (debug builds only). This method will not
    return.
*/
void
SysFunc::Exit(int exitCode)
{
    // first produce a RefCount leak report
    #if NEBULA3_DEBUG
    Core::RefCounted::DumpRefCountingLeaks();
    #endif

    // shutdown the C runtime, this cleans up static objects but doesn't shut 
    // down the process
    _cexit();

    // @todo: report mem leaks

    // finally terminate the process
    ExitProcess(exitCode);
}

//------------------------------------------------------------------------------
/**
    This displays a Win32 error message box and quits the program
*/
void
SysFunc::Error(const char* error)
{
    #ifdef _DEBUG
    OutputDebugString(error);
    #endif
    HWND hwnd = FindWindow(NEBULA3_WINDOW_CLASS, NULL);
    if (hwnd)
    {
        ShowWindow(hwnd, SW_MINIMIZE);
    }
    MessageBox(NULL, error, "NEBULA3 SYSTEM ERROR", MB_OK|MB_APPLMODAL|MB_SETFOREGROUND|MB_TOPMOST|MB_ICONERROR);
    Debug::MiniDump::WriteMiniDump();
    abort();
}

//------------------------------------------------------------------------------
/**
    Sleep for a specified amount of seconds, give up time slice.
*/
void
SysFunc::Sleep(double sec)
{
    int milliSecs = int(sec * 1000.0);
    ::Sleep(milliSecs);
}

//------------------------------------------------------------------------------
/**
    Returns the last Win32 system error as string.
*/
String
SysFunc::GetLastError()
{
    // get system error code
    DWORD err = ::GetLastError();
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL);
    String msg = (const char*) lpMsgBuf;
    LocalFree(lpMsgBuf);
    return msg;
}

//------------------------------------------------------------------------------
/**
    Put a message on the debug console.
*/
void
SysFunc::DebugOut(const char* msg)
{
    OutputDebugString(msg);
}

} // namespace Win32