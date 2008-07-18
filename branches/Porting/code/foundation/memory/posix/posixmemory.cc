//------------------------------------------------------------------------------
//  posixmemory.cc
//  (C) 2006 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/types.h"
#include "memory/heap.h"

namespace Memory
{
#if NEBULA3_MEMORY_STATS
int volatile AllocCount = 0;
int volatile AllocSize = 0;

//------------------------------------------------------------------------------
/**
    Debug function which validates the process heap and all local heaps. 
    Stops the program if something is wrong. 
*/
bool
Validate()
{
    if (0 == PosixProcessHeap)
    {
        PosixProcessHeap = GetProcessHeap();
    }
    bool res = (0 != HeapValidate(PosixProcessHeap, 0, NULL));
    res &= Heap::ValidateAllHeaps();
    return res;
}

#endif
} // namespace Memory


