//------------------------------------------------------------------------------
//  win32minidump.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debug/win32/win32minidump.h"
#include "io/win32/win32fswrapper.h"
#include "timing/calendartime.h"

namespace Win32
{
using namespace Util;
using namespace Timing;
using namespace IO;

//------------------------------------------------------------------------------
/**
    This method is called by n_assert() and n_error() to write out
    a minidump file.
*/
bool
Win32MiniDump::WriteMiniDump()
{
#if (NEBULA3_ENABLE_MINIDUMPS)
    String dumpFilename = BuildMiniDumpFilename();
    if (dumpFilename.IsValid())
    {
        Win32FSWrapper::Handle hFile = Win32FSWrapper::OpenFile(dumpFilename, Stream::WriteAccess, Stream::Sequential);
        if (NULL != hFile)
        {
            HANDLE hProc = GetCurrentProcess();
            DWORD procId = GetCurrentProcessId();
            BOOL res = MiniDumpWriteDump(hProc, procId, hFile, MiniDumpNormal, NULL, NULL, NULL);
            Win32FSWrapper::CloseFile(hFile);
            return true;
        }
    }
#endif
    return false;
}

//------------------------------------------------------------------------------
/**
*/
String
Win32MiniDump::BuildMiniDumpFilename()
{
    String dumpFilename;

    // get our module filename directly from windows
    TCHAR buf[512];
    Memory::Clear(buf, sizeof(buf));
    DWORD numBytes = GetModuleFileName(NULL, buf, sizeof(buf) - 1);
    if (numBytes > 0)
    {
        String modulePath(buf);
        String moduleName = modulePath.ExtractFileName();
        moduleName.StripFileExtension();
        modulePath = modulePath.ExtractToLastSlash();

        // get the current calender time
        CalendarTime calTime = CalendarTime::GetLocalTime();
        String timeStr = CalendarTime::Format("{YEAR}-{MONTH}-{DAY}_{HOUR}-{MINUTE}-{SECOND}", calTime);
        
        // build the dump filename
        dumpFilename.Format("%s%s_%s.dmp", modulePath.AsCharPtr(), moduleName.AsCharPtr(), timeStr.AsCharPtr());
    }
    return dumpFilename;
}

} // namespace Debug
