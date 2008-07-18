#pragma once
#ifndef WIN32_WIN32HEAP_H
#define WIN32_WIN32HEAP_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32Heap
  
    Win32 implementation of the class Memory::Heap using the Win32-Heap 
    functions. Generally switches on the Low-Fragmentation-Heap, since this
    seems generally suitable for most C++ applications.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "threading/interlocked.h"
#include "threading/criticalsection.h"
#include "util/array.h"
#include "util/list.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32Heap
{
public:
    /// static setup method (called by Util::Setup)
    static void Setup();
    /// constructor (name must be static string!)
    Win32Heap(const char* name);
    /// destructor
    ~Win32Heap();
    /// get heap name
    const char* GetName() const;
    /// allocate a block of memory from the heap
    void* Alloc(size_t size);
    /// re-allocate a block of memory
    void* Realloc(void* ptr, size_t newSize);
    /// free a block of memory which has been allocated from this heap
    void Free(void* ptr);

    #if NEBULA3_MEMORY_STATS
    /// heap stats structure
    struct Stats
    {
        const char* name;
        int allocCount;
        int allocSize;
    };
    /// gather stats from all existing heaps
    static Util::Array<Stats> GetAllHeapStats();
    /// validate all heaps
    static bool ValidateAllHeaps();
    /// validate the heap (only useful in Debug builds)
    bool ValidateHeap() const;
    /// get the current alloc count
    int GetAllocCount() const;
    /// get the current alloc size
    int GetAllocSize() const;
    #endif

private:
    /// default constructor not allowed
    Win32Heap();

    HANDLE heap;
    const char* name;

    #if NEBULA3_MEMORY_STATS
    int volatile allocCount;
    int volatile allocSize;
    static Threading::CriticalSection*  criticalSection;
    static Util::List<Win32Heap*>* list;
    Util::List<Win32Heap*>::Iterator listIterator;
    #endif
};

//------------------------------------------------------------------------------
/**
*/
inline const char*
Win32Heap::GetName() const
{
    n_assert(0 != this->name);
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void*
Win32Heap::Alloc(size_t size)
{
    #if NEBULA3_MEMORY_STATS
    Threading::Interlocked::Increment(this->allocCount);
    Threading::Interlocked::Add(this->allocSize, int(size));
    #endif
    return HeapAlloc(this->heap, HEAP_GENERATE_EXCEPTIONS, size);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void*
Win32Heap::Realloc(void* ptr, size_t size)
{
    #if NEBULA3_MEMORY_STATS
    size_t curSize = HeapSize(this->heap, 0, ptr);
    Threading::Interlocked::Add(this->allocSize, int(size - curSize));
    #endif
    return HeapReAlloc(this->heap, HEAP_GENERATE_EXCEPTIONS, ptr, size);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
Win32Heap::Free(void* ptr)
{
    n_assert(0 != ptr);
    #if NEBULA3_MEMORY_STATS
    size_t size = HeapSize(this->heap, 0, ptr);
    Threading::Interlocked::Add(this->allocSize, -int(size));
    Threading::Interlocked::Decrement(this->allocCount);
    #endif
    BOOL success = HeapFree(this->heap, 0, ptr);
    n_assert(0 != success);
}

} // namespace Win32
//------------------------------------------------------------------------------
#endif
