#pragma once
#ifndef WIN32_WIN32MINIDUMP_H
#define WIN32_WIN32MINIDUMP_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32MiniDump
  
    Win32 implementation of MiniDump.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32MiniDump
{
public:
    /// write a mini dump
    static bool WriteMiniDump();

private:
    /// build a filename for the dump file
    static Util::String BuildMiniDumpFilename();
};
 
} // namespace Win32
//------------------------------------------------------------------------------
#endif
