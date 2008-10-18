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
#if (__WIN32__ || __XBOX360__)
#include "io/win360/win360filetime.h"
namespace IO
{
class FileTime : public Win360::Win360FileTime
{ };
}
#elif __WII__
#include "io/wii/wiifiletime.h"
namespace IO
{
class FileTime : public Wii::WiiFileTime
{ };
}
#else
#error "FileTime class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif


