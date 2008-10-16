#pragma once
#ifndef SYSTEM_SYSTEMINFO_H
#define SYSTEM_SYSTEMINFO_H
//------------------------------------------------------------------------------
/**
    @class System::SystemInfo
    
    Provides information about the host system.
    
    (C) 2008 Radon Labs GmbH
*/
#if __WIN32__
#include "system/win32/win32systeminfo.h"
namespace System
{
class SystemInfo : public Win32::Win32SystemInfo {};
}
#elif __APPLE__
#include "system/darwin/darwinsysteminfo.h"
namespace System
{
class SystemInfo : public Darwin::DarwinSystemInfo {};
}
#else
#error "System::SystemInfo not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
