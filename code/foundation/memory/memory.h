#pragma once
#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H
//------------------------------------------------------------------------------
/**
    @class Memory::Memory
  
    Implements a private heap.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "memory/win32/win32memory.h"
#elif __XBOX360__
#include "memory/xbox360/xbox360memory.h"
#elif __WII__
#include "memory/wii/wiimemory.h"
#elif __APPLE__
#include "memory/posix/posixmemory.h"
#else
#error "IMPLEMENT ME!"
#endif
#endif
