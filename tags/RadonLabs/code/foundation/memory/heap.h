#pragma once
#ifndef MEMORY_HEAP_H
#define MEMORY_HEAP_H
//------------------------------------------------------------------------------
/**
    @class Memory::Heap
  
    Implements a private heap.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "memory/win32/win32heap.h"
namespace Memory
{
typedef Win32::Win32Heap Heap;
}
#elif __XBOX360__
#include "memory/xbox360/xbox360heap.h"
namespace Memory
{
typedef Xbox360::Xbox360Heap Heap;
}
#elif __WII__
#include "memory/wii/wiiheap.h"
namespace Memory
{
typedef Wii::WiiHeap Heap;
}
#else
#error "IMPLEMENT ME!"
#endif
#endif
