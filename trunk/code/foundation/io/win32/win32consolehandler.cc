//------------------------------------------------------------------------------
//  win32consolehandler.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/win32/win32consolehandler.h"
#include "core/coreserver.h"
#include "core/sysfunc.h"
#include "threading/thread.h"

namespace Win32
{
ImplementClass(Win32::Win32ConsoleHandler, 'W32C', IO::ConsoleHandler);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Win32ConsoleHandler::Win32ConsoleHandler()
{
    // obtain handle to stdout
    this->stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    this->stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    this->stderrHandle = GetStdHandle(STD_ERROR_HANDLE);
    n_assert(INVALID_HANDLE_VALUE != this->stdoutHandle);
    n_assert(INVALID_HANDLE_VALUE != this->stdinHandle);
    n_assert(INVALID_HANDLE_VALUE != this->stderrHandle);
}

//------------------------------------------------------------------------------
/**
*/
void
Win32ConsoleHandler::Print(const String& s)
{
    DWORD charsWritten;
    const char* threadName = Threading::Thread::GetMyThreadName();
    String msg;
    if (0 == threadName)
    {
        // a message from the main thread
        WriteConsole(this->stdoutHandle, s.AsCharPtr(), s.Length(), &charsWritten, NULL);
        #ifdef _DEBUG
        OutputDebugString(s.AsCharPtr());
        #endif
    }
    else
    {
        // a message from another thread, add the thread name to the message
        msg.Format("[%s] %s", Threading::Thread::GetMyThreadName(), s.AsCharPtr());
        WriteConsole(this->stdoutHandle, msg.AsCharPtr(), msg.Length(), &charsWritten, NULL);
        #ifdef _DEBUG
        OutputDebugString(msg.AsCharPtr());
        #endif
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Win32ConsoleHandler::DebugOut(const String& s)
{
    const char* threadName = Threading::Thread::GetMyThreadName();
    String msg;
    if (0 == threadName)
    {
        // a message from the main thread
        OutputDebugString(s.AsCharPtr());
    }
    else
    {
        String msg;
        msg.Format("[%s] %s", Threading::Thread::GetMyThreadName(), s.AsCharPtr());
        OutputDebugString(s.AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Win32ConsoleHandler::Error(const String& msg)
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
    DWORD charsWritten;
    WriteConsole(this->stderrHandle, str.AsCharPtr(), str.Length(), &charsWritten, NULL);
    Core::SysFunc::Error(str.AsCharPtr());
}

//------------------------------------------------------------------------------
/**
*/
void
Win32ConsoleHandler::Warning(const String& s)
{
    DWORD charsWritten;
    const char* threadName = Threading::Thread::GetMyThreadName();
    String msg;
    if (0 == threadName)
    {
        // a message from the main thread
        WriteConsole(this->stderrHandle, s.AsCharPtr(), s.Length(), &charsWritten, NULL);
        #ifdef _DEBUG
        OutputDebugString(s.AsCharPtr());
        #endif
    }
    else
    {
        // a message from another thread, add the thread name to the message
        msg.Format("[%s] %s", Threading::Thread::GetMyThreadName(), s.AsCharPtr());
        WriteConsole(this->stderrHandle, msg.AsCharPtr(), msg.Length(), &charsWritten, NULL);
        #ifdef _DEBUG
        OutputDebugString(msg.AsCharPtr());
        #endif
    }
}

//------------------------------------------------------------------------------
/**
    Since we are blocking the app waiting for user input, we always provide 
    input.
*/
bool
Win32ConsoleHandler::HasInput()
{
    return true;
}

//------------------------------------------------------------------------------
/**
    Get user input from the console.
*/
String
Win32ConsoleHandler::GetInput()
{
    String result;
    const DWORD bufNumChars = 4096;
    DWORD numCharsRead = 0;
    TCHAR buf[bufNumChars];
    BOOL success = ReadConsole(this->stdinHandle, buf, bufNumChars, &numCharsRead, NULL);
    if (success && (numCharsRead > 0))
    {
        result.Set(buf, numCharsRead);
        result.TrimRight("\n\r");
    }
    return result;
}

}; // namespace IO