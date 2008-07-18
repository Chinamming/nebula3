#pragma once
#ifndef MEMORY_POSIXMEMORY_H
#define MEMORY_POSIXMEMORY_H
//------------------------------------------------------------------------------
/**
    @file memory/posix/posixmemory.h

    Low level memory functions for the Posix platform.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#include "core/debug.h"
#include "threading/interlocked.h"

namespace Memory
{
#if NEBULA3_MEMORY_STATS
extern int volatile AllocCount;
extern int volatile AllocSize;
#endif

//------------------------------------------------------------------------------
/**
    Allocate a block of memory from the process heap.
*/
__forceinline void*
Alloc(size_t size)
{
    #if NEBULA3_MEMORY_STATS
    Threading::Interlocked::Increment(AllocCount);
    Threading::Interlocked::Add(AllocSize, int(size));
    #endif
    return malloc(size);
}

//------------------------------------------------------------------------------
/**
    Reallocate a block of memory.
*/
__forceinline void*
Realloc(void* ptr, size_t size)
{
    #if NEBULA3_MEMORY_STATS
    size_t curSize = HeapSize(PosixProcessHeap, 0, ptr);
    Threading::Interlocked::Add(AllocSize, int(size - curSize));
    #endif
    return realloc(ptr, size);
}

//------------------------------------------------------------------------------
/**
    Free a chunk of memory from the process heap.
*/
__forceinline void
Free(void* ptr)
{
    n_assert(0 != ptr);
    #if NEBULA3_MEMORY_STATS
    n_assert(0 != ptr);
    size_t size = HeapSize(PosixProcessHeap, 0, ptr);
    Threading::Interlocked::Add(AllocSize, -int(size));
    Threading::Interlocked::Decrement(AllocCount);
    #endif
    free(ptr);
}

//------------------------------------------------------------------------------
/**
    Copy a chunk of memory (note the argument order is different 
    from memcpy()!!!)
*/
__forceinline void
Copy(const void* from, void* to, size_t numBytes)
{
    if (numBytes > 0)
    {
        n_assert(0 != from);
        n_assert(0 != to);
        n_assert(from != to);
        memcpy(to, from, numBytes);
    }
}

//------------------------------------------------------------------------------
/**
    Overwrite a chunk of memory with 0's.
*/
__forceinline void
Clear(void* ptr, size_t numBytes)
{
    memset(ptr, 0, numBytes);
}

//------------------------------------------------------------------------------
/**
    Duplicate a 0-terminated string.
*/
__forceinline char*
DuplicateCString(const char* from)
{
    n_assert(0 != from);
    size_t len = (unsigned int) strlen(from) + 1;
    char* to = (char*) Memory::Alloc(len);
    Memory::Copy((void*)from, to, len);
    return to;
}

//------------------------------------------------------------------------------
/**
    Get the system's total current memory, this does not only include
    Nebula3's memory allocations but the memory usage of the entire system.
*/
struct MemoryStatus
{
    unsigned int totalPhysical;
    unsigned int availPhysical;
    unsigned int totalVirtual;
    unsigned int availVirtual;
};

inline MemoryStatus
GetMemoryStatus()
{
    MemoryStatus result;
    result.totalPhysical = (unsigned int) 0;
    result.availPhysical = (unsigned int) 0;
    result.totalVirtual  = (unsigned int) 0;
    result.availVirtual  = (unsigned int) 0;
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
    return Memory::Alloc(size);
}

//------------------------------------------------------------------------------
/**
    Replacement global new[] operator.
*/
__forceinline void*
__cdecl operator new[](size_t size)
{
    return Memory::Alloc(size);
}

//------------------------------------------------------------------------------
/**
    Replacement global delete operator.
*/
__forceinline void
__cdecl operator delete(void* p)
{
    Memory::Free(p);
}

//------------------------------------------------------------------------------
/**
    Replacement global delete[] operator.
*/
__forceinline void
__cdecl operator delete[](void* p)
{
    Memory::Free(p);
}

#define n_new(type) new type
#define n_new_array(type,size) new type[size]
#define n_delete(ptr) delete ptr
#define n_delete_array(ptr) delete[] ptr
//------------------------------------------------------------------------------
#endif

