//------------------------------------------------------------------------------
//  win32heap.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "memory/win32/win32heap.h"

namespace Win32
{
using namespace Threading;
using namespace Util;

#if NEBULA3_MEMORY_STATS
List<Win32Heap*>* Win32Heap::list = 0;
CriticalSection* Win32Heap::criticalSection = 0;
#endif

//------------------------------------------------------------------------------
/**
    This method must be called at the beginning of the application because
    any threads are spawned (usually called by Util::Setup().
*/
void
Win32Heap::Setup()
{
    #if NEBULA3_MEMORY_STATS
    n_assert(0 == list);
    n_assert(0 == criticalSection);
    list = n_new(List<Win32Heap*>);
    criticalSection = n_new(CriticalSection);
    #endif
}

//------------------------------------------------------------------------------
/**
*/
Win32Heap::Win32Heap(const char* heapName)
{
    n_assert(0 != heapName);
    this->name = heapName;
    this->heap = HeapCreate(0, 0, 0);
    n_assert(0 != this->heap);
    
    // enable low-fragmentatio-heap
    ULONG heapFragValue = 2;
    HeapSetInformation(this->heap, HeapCompatibilityInformation, &heapFragValue, sizeof(heapFragValue));

    // link into Heap list
    #if NEBULA3_MEMORY_STATS
    n_assert(0 != criticalSection);
    this->allocCount = 0;
    this->allocSize  = 0;
    criticalSection->Enter();
    this->listIterator = list->AddBack(this);
    criticalSection->Leave();
    #endif
}

//------------------------------------------------------------------------------
/**
*/
Win32Heap::~Win32Heap()
{
    BOOL success = HeapDestroy(this->heap);
    n_assert(0 != success);
    this->heap = 0;

    // unlink from Heap list
    #if NEBULA3_MEMORY_STATS
    n_assert(0 == this->allocCount);
    n_assert(0 != criticalSection);
    n_assert(0 != this->listIterator);
    criticalSection->Enter();
    list->Remove(this->listIterator);
    criticalSection->Leave();
    this->listIterator = 0;
    #endif   
}

#if NEBULA3_MEMORY_STATS
//------------------------------------------------------------------------------
/**
    Validate the heap. This walks over the heap's memory block and checks
    the control structures. If somethings wrong the function will
    stop the program, otherwise the functions returns true.
*/
bool
Win32Heap::ValidateHeap() const
{
    BOOL result = HeapValidate(this->heap, 0, NULL);
    return (0 != result);
}


//------------------------------------------------------------------------------
/**
*/
int
Win32Heap::GetAllocCount() const
{
    return this->allocCount;
}

//------------------------------------------------------------------------------
/**
*/
int
Win32Heap::GetAllocSize() const
{
    return this->allocSize;
}

//------------------------------------------------------------------------------
/**
*/
Array<Win32Heap::Stats>
Win32Heap::GetAllHeapStats()
{
    n_assert(0 != criticalSection);
    Array<Stats> result;
    criticalSection->Enter();
    List<Win32Heap*>::Iterator iter;
    for (iter = list->Begin(); iter != list->End(); iter++)
    {
        Stats stats;
        stats.name       = (*iter)->GetName();
        stats.allocCount = (*iter)->GetAllocCount();
        stats.allocSize  = (*iter)->GetAllocSize();        
        result.Append(stats);
    }
    criticalSection->Leave();
    return result;
}

//------------------------------------------------------------------------------
/**
    This static method calls the ValidateHeap() method on all heaps.
*/
bool
Win32Heap::ValidateAllHeaps()
{
    n_assert(0 != criticalSection);
    criticalSection->Enter();
    bool result = true;
    List<Win32Heap*>::Iterator iter;
    for (iter = list->Begin(); iter != list->End(); iter++)
    {
        result &= (*iter)->ValidateHeap();
    }
    criticalSection->Leave();
    return result;
}
#endif // NEBULA3_MEMORY_STATS

} // namespace Memory
