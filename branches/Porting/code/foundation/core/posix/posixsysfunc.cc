//------------------------------------------------------------------------------
//  sysfunc.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/posix/posixsysfunc.h"
#include "core/refcounted.h"
#include "util/blob.h"
#include "util/guid.h"
#include <errno.h>

namespace Posix
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

    // @todo: report mem leaks

    // finally terminate the process
    exit(exitCode);
}

//------------------------------------------------------------------------------
/**
*/
void
SysFunc::Error(const char* error)
{
    fprintf(stderr, error);
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
    usleep(milliSecs);
}

//------------------------------------------------------------------------------
/**
    Returns the last system error as string.
*/
String
SysFunc::GetLastError()
{
    // get system error code
    char msgBuf[2048];
    if (strerror_r(errno, msgBuf, 2048) == 0)
    {
        return msgBuf;
    }
    return "Missing system message.";
}

//------------------------------------------------------------------------------
/**
    Put a message on the debug console.
*/
void
SysFunc::DebugOut(const char* msg)
{
    fprintf(stderr, msg);
}

} // namespace Posix
