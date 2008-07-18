#pragma once
#ifndef IO_FILETIME_H
#define IO_FILETIME_H
//------------------------------------------------------------------------------
/**
    @class IO::FileTime
    
    Defines a file-access timestamp.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "io/win32/win32filetime.h"
namespace IO
{
class FileTime : public Win32::Win32FileTime
{ };
}
#elif __XBOX360__
#include "io/xbox360/xbox360filetime.h"
namespace IO
{
class FileTime : public Xbox360::Xbox360FileTime
{ };
}
#elif __WII__
#include "io/wii/wiifiletime.h"
namespace IO
{
class FileTime : public Wii::WiiFileTime
{ };
}
#elif __APPLE__
#include "io/posix/posixfiletime.h"
namespace IO
{
class FileTime : public Posix::PosixFileTime
{ };
}
#else
#error "FileTime class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif


