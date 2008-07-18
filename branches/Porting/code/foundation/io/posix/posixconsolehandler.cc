//------------------------------------------------------------------------------
//  posixconsolehandler.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/posix/posixconsolehandler.h"
#include "core/coreserver.h"
#include "core/sysfunc.h"
#include "threading/thread.h"

namespace Posix
{
ImplementClass(Posix::PosixConsoleHandler, 'POSC', IO::ConsoleHandler);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
PosixConsoleHandler::PosixConsoleHandler()
{
    // obtain handle to stdout
    this->stdoutHandle = stdout;
    this->stdinHandle = stdin;
    this->stderrHandle = stderr;
    n_assert(NULL != this->stdoutHandle);
    n_assert(NULL != this->stdinHandle);
    n_assert(NULL != this->stderrHandle);
}

//------------------------------------------------------------------------------
/**
*/
void
PosixConsoleHandler::Print(const String& s)
{
    const char* threadName = Threading::Thread::GetMyThreadName();
    String msg;
    if (0 == threadName)
    {
        // a message from the main thread
        fprintf(this->stdoutHandle, s.AsCharPtr());
    }
    else
    {
        // a message from another thread, add the thread name to the message
        msg.Format("[%s] %s", Threading::Thread::GetMyThreadName(), s.AsCharPtr());
        fprintf(this->stdoutHandle, msg.AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PosixConsoleHandler::DebugOut(const String& s)
{
    const char* threadName = Threading::Thread::GetMyThreadName();
    String msg;
    if (0 == threadName)
    {
        // a message from the main thread
        fprintf(this->stderrHandle, s.AsCharPtr());
    }
    else
    {
        String msg;
        msg.Format("[%s] %s", Threading::Thread::GetMyThreadName(), s.AsCharPtr());
        fprintf(this->stderrHandle, msg.AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PosixConsoleHandler::Error(const String& msg)
{
    const char* threadName = Threading::Thread::GetMyThreadName();
    const char* appName = "???";
    if (0 == threadName)
    {
        threadName = "Main";
    }
    if (Core::CoreServer::HasInstance())
    {
        appName = Core::CoreServer::Instance()->GetAppName().AsCharPtr();
    }
    String str;
    str.Format("*** ERROR ***\nApplication: %s\nThread: %s\nError: %s", appName, threadName, msg.AsCharPtr());
    fprintf(this->stderrHandle, str.AsCharPtr());
    Core::SysFunc::Error(str.AsCharPtr());
}

//------------------------------------------------------------------------------
/**
*/
void
PosixConsoleHandler::Warning(const String& s)
{
    const char* threadName = Threading::Thread::GetMyThreadName();
    String msg;
    if (0 == threadName)
    {
        // a message from the main thread
        fprintf(this->stderrHandle, s.AsCharPtr());
    }
    else
    {
        // a message from another thread, add the thread name to the message
        msg.Format("[%s] %s", Threading::Thread::GetMyThreadName(), s.AsCharPtr());
        fprintf(this->stderrHandle, msg.AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
    Since we are blocking the app waiting for user input, we always provide 
    input.
*/
bool
PosixConsoleHandler::HasInput()
{
    return true;
}

//------------------------------------------------------------------------------
/**
    Get user input from the console.
*/
String
PosixConsoleHandler::GetInput()
{
    String result;
    char buf[4096];
    size_t numCharsRead = fread(buf, sizeof(char), sizeof(buf), this->stdinHandle);
    if (numCharsRead > 0)
    {
        result.Set(buf, numCharsRead);
        result.TrimRight("\n\r");
    }
    return result;
}

}; // namespace IO