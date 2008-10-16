#pragma once
#ifndef WIN360_WIN360HEAP_H
#define WIN360_WIN360HEAP_H
//------------------------------------------------------------------------------
/**
    @class Win360::Win360Heap
  
    Win32/Xbox360 implementation of the class Memory::Heap. Under Win32,
    the LowFragmentationHeap feature is generally turned on.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "threading/interlocked.h"
#include "threading/criticalsection.h"
#include "util/array.h"
#include "util/list.h"

//------------------------------------------------------------------------------
namespace Win360
{
class Win360Heap
{
public:
    /// static setup method (called by Util::Setup)
    static void Setup();
    /// constructor (name must be static string!)
    Win360Heap(const char* name);
    /// destructor
    ~Win360Heap();
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
    Win360Heap();

    HANDLE heap;
    const char* name;

    #if NEBULA3_MEMORY_STATS
    int volatile allocCount;
    int volatile allocSize;
    static Threading::CriticalSection*  criticalSection;
    static Util::List<Win360Heap*>* list;
    Util::List<Win360Heap*>::Iterator listIterator;
    #endif
};

//------------------------------------------------------------------------------
/**
*/
inline const char*
Win360Heap::GetName() const
{
    n_assert(0 != this->name);
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void*
Win360Heap::Alloc(size_t size)
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
Win360Heap::Realloc(void* ptr, size_t size)
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
Win360Heap::Free(void* ptr)
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

} // namespace Win360Heap
//------------------------------------------------------------------------------
#endif
