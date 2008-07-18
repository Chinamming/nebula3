//------------------------------------------------------------------------------
//  win32memory.cc
//  (C) 2006 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/types.h"
#include "memory/heap.h"

namespace Memory
{
HANDLE volatile Win32ProcessHeap = 0;
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
    if (0 == Win32ProcessHeap)
    {
        Win32ProcessHeap = GetProcessHeap();
    }
    bool res = (0 != HeapValidate(Win32ProcessHeap, 0, NULL));
    res &= Heap::ValidateAllHeaps();
    return res;
}

#endif
} // namespace Memory


