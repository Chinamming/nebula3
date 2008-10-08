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
#else
#error "System::SystemInfo not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
