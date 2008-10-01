#pragma once
#ifndef MEMORY_WIN32MEMORY_H
#define MEMORY_WIN32MEMORY_H
//------------------------------------------------------------------------------
/**
    @file memory/win32/win32memory.h

    Low level memory functions for the Win32 platform.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#include "core/debug.h"
#include "threading/interlocked.h"
#include "memory/win32/win32memoryconfig.h"

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
    n_assert((heapType < NumHeapTypes) && (0 != Heaps[heapType]));
    void* allocPtr =  HeapAlloc(Heaps[heapType], HEAP_GENERATE_EXCEPTIONS, size);
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
    n_assert(0 != ptr);
    n_assert((heapType < NumHeapTypes) && (0 != Heaps[heapType]));
    #if NEBULA3_MEMORY_STATS
        SIZE_T size = HeapSize(Heaps[heapType], 0, ptr);
        Threading::Interlocked::Add(TotalAllocSize, -int(size));
        Threading::Interlocked::Decrement(TotalAllocCount);
        Threading::Interlocked::Add(HeapTypeAllocSize[heapType], -int(size));
        Threading::Interlocked::Decrement(HeapTypeAllocCount[heapType]);
    #endif
    HeapFree(Heaps[heapType], 0, ptr);
}

//------------------------------------------------------------------------------
/**
    Copy a chunk of memory (note the argument order is different from memcpy()!!!)
*/
__forceinline void
Copy(const void* from, void* to, size_t numBytes)
{
    if (numBytes > 0)
    {
        n_assert(0 != from);
        n_assert(0 != to);
        n_assert(from != to);
        CopyMemory(to, from, numBytes);
    }
}

//------------------------------------------------------------------------------
/**
    Overwrite a chunk of memory with 0's.
*/
__forceinline void
Clear(void* ptr, size_t numBytes)
{
    ZeroMemory(ptr, numBytes);
}

//------------------------------------------------------------------------------
/**
    Fill memory with a specific byte.
*/
__forceinline void
Fill(void* ptr, size_t numBytes, unsigned char value)
{
    FillMemory(ptr, numBytes, value);
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
    