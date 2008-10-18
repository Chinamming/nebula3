#pragma once
#ifndef MEMORY_WIN360MEMORY_H
#define MEMORY_WIN360MEMORY_H
//------------------------------------------------------------------------------
/**
    @file memory/win360/win360memory.h
    
    Memory subsystem features which are identical on Win32 and Xbox360.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/config.h"
#include "core/debug.h"
#include "threading/interlocked.h"
#include "memory/win360/win360memoryconfig.h"
#if __WIN32__
#include "memory/win32/win32memory.h"
#elif __XBOX360__
#include "memory/xbox360/xbox360memory.h"
#endif

namespace Memory
{
#if NEBULA3_MEMORY_STATS
extern int volatile TotalAllocCount;
extern int volatile TotalAllocSize;
extern int volatile HeapTypeAllocCount[NumHeapTypes];
extern int volatile HeapTypeAllocSize[NumHeapTypes];
#endif

//------------------------------------------------------------------------------
/**
    Allocate a block of memory from the process heap.
*/
__forceinline void*
Alloc(HeapType heapType, size_t size)
{
    n_assert(heapType < NumHeapTypes);
    void* allocPtr = 0;
    #if __XBOX360__
    if (Xbox360GraphicsHeap == heapType)
    {
        allocPtr = Xbox360AllocPhysicalMemory(size, XALLOC_MEMPROTECT_WRITECOMBINE);
    }
    else if (Xbox360AudioHeap == heapType)
    {
        allocPtr = Xbox360AllocPhysicalMemory(size, 0);
    }
    else
    #endif __XBOX360__
    {
        n_assert(0 != Heaps[heapType]);
        allocPtr =  HeapAlloc(Heaps[heapType], HEAP_GENERATE_EXCEPTIONS, size);
    }
    #if NEBULA3_MEMORY_STATS
        SIZE_T s = HeapSize(Heaps[heapType], 0, allocPtr);
        Threading::Interlocked::Increment(TotalAllocCount);
        Threading::Interlocked::Add(TotalAllocSize, int(s));
        Threading::Interlocked::Increment(HeapTypeAllocCount[heapType]);
        Threading::Interlocked::Add(HeapTypeAllocSize[heapType], int(s));
    #endif
    return allocPtr;
}

//------------------------------------------------------------------------------
/**
    Reallocate a block of memory.
*/
__forceinline void*
Realloc(HeapType heapType, void* ptr, size_t size)
{
    n_assert((heapType != Xbox360GraphicsHeap) && (heapType != Xbox360AudioHeap));
    n_assert((heapType < NumHeapTypes) && (0 != Heaps[heapType]));
    #if NEBULA3_MEMORY_STATS
        SIZE_T oldSize = HeapSize(Heaps[heapType], 0, ptr);
    #endif
    void* allocPtr = HeapReAlloc(Heaps[heapType], HEAP_GENERATE_EXCEPTIONS, ptr, size);
    #if NEBULA3_MEMORY_STATS
        SIZE_T newSize = HeapSize(Heaps[heapType], 0, allocPtr);
        Threading::Interlocked::Add(TotalAllocSize, int(newSize - oldSize));
        Threading::Interlocked::Add(HeapTypeAllocSize[heapType], int(newSize - oldSize));
    #endif
    return allocPtr;
}

//------------------------------------------------------------------------------
/**
    Free a chunk of memory from the process heap.
*/
__forceinline void
Free(HeapType heapType, void* ptr)
{
    // D3DX on the 360 likes to call the delete operator with a 0 pointer
    if (0 != ptr)
    {
        n_assert(heapType < NumHeapTypes);
        #if NEBULA3_MEMORY_STATS
            SIZE_T size = 0;
        #endif    
        #if __XBOX360__
        if (Xbox360GraphicsHeap == heapType)
        {
            #if NEBULA3_MEMORY_STATS
                size = Xbox360PhysicalMemorySize(ptr, XALLOC_MEMPROTECT_WRITECOMBINE);
            #endif
            Xbox360FreePhysicalMemory(ptr, XALLOC_MEMPROTECT_WRITECOMBINE);
        }
        else if (Xbox360AudioHeap == heapType)
        {
            #if NEBULA3_MEMORY_STATS
                size = Xbox360PhysicalMemorySize(ptr, 0);
            #endif
            Xbox360FreePhysicalMemory(ptr, 0);
        }
        else
        #endif // __XBOX360__
        {
            n_assert(0 != Heaps[heapType]);
            #if NEBULA3_MEMORY_STATS
                size = HeapSize(Heaps[heapType], 0, ptr);
            #endif
            HeapFree(Heaps[heapType], 0, ptr);
        }
        #if NEBULA3_MEMORY_STATS
            Threading::Interlocked::Add(TotalAllocSize, -int(size));
            Threading::Interlocked::Decrement(TotalAllocCount);
            Threading::Interlocked::Add(HeapTypeAllocSize[heapType], -int(size));
            Threading::Interlocked::Decrement(HeapTypeAllocCount[heapType]);
        #endif
    }
}

//------------------------------------------------------------------------------
/**
    Duplicate a 0-terminated string. The memory will be allocated from
    the StringHeap (important when freeing the memory!)
*/
__forceinline char*
DuplicateCString(const char* from)
{
    n_assert(0 != from);
    size_t len = (unsigned int) strlen(from) + 1;
    char* to = (char*) Memory::Alloc(Memory::StringHeap, len);
    Memory::Copy((void*)from, to, len);
    return to;
}

//------------------------------------------------------------------------------
/**
    Test if 2 areas of memory area overlapping.
*/
inline bool
IsOverlapping(const unsigned char* srcPtr, size_t srcSize, const unsigned char* dstPtr, size_t dstSize)
{
    if (srcPtr == dstPtr)
    {
        return true;
    }
    else if (srcPtr > dstPtr)
    {
        return (srcPtr + srcSize) > dstPtr;
    }
    else
    {
        return (dstPtr + dstSize) > srcPtr;
    }
}

//------------------------------------------------------------------------------
/**
    Get the system's total current memory, this does not only include
    Nebula3's memory allocations but the memory usage of the entire system.
*/
struct TotalMemoryStatus
{
    unsigned int totalPhysical;
    unsigned int availPhysical;
    unsigned int totalVirtual;
    unsigned int availVirtual;
};

inline TotalMemoryStatus
GetTotalMemoryStatus()
{
    MEMORYSTATUS stats = { NULL };
    GlobalMemoryStatus(&stats);
    TotalMemoryStatus result;
    result.totalPhysical = (unsigned int) stats.dwTotalPhys;
    result.availPhysical = (unsigned int) stats.dwAvailPhys;
    result.totalVirtual  = (unsigned int) stats.dwTotalVirtual;
    result.availVirtual  = (unsigned int) stats.dwAvailVirtual;
    return result;
}

//------------------------------------------------------------------------------
/**
    Debug function which validates the process heap. This will NOT check
    local heaps (call Heap::ValidateAllHeaps() for this). 
    Stops the program if something is wrong. 
*/
#if NEBULA3_MEMORY_STATS
extern bool Validate();
#endif
} // namespace Memory

#ifdef new
#undef new
#endif

#ifdef delete
#undef delete
#endif

//------------------------------------------------------------------------------
/**
    Replacement global new operator.
*/
__forceinline void*
__cdecl operator new(size_t size)
{
    return Memory::Alloc(Memory::ObjectHeap, size);
}

//------------------------------------------------------------------------------
/**
    Replacement global new[] operator.
*/
__forceinline void*
__cdecl operator new[](size_t size)
{
    return Memory::Alloc(Memory::ObjectArrayHeap, size);
}

//------------------------------------------------------------------------------
/**
    Replacement global delete operator.
*/
__forceinline void
__cdecl operator delete(void* p)
{
    Memory::Free(Memory::ObjectHeap, p);
}

//------------------------------------------------------------------------------
/**
    Replacement global delete[] operator.
*/
__forceinline void
__cdecl operator delete[](void* p)
{
    Memory::Free(Memory::ObjectArrayHeap, p);
}

#define n_new(type) new type
#define n_new_array(type,size) new type[size]
#define n_delete(ptr) delete ptr
#define n_delete_array(ptr) delete[] ptr
//------------------------------------------------------------------------------
#endif

