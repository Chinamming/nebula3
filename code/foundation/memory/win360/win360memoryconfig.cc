//------------------------------------------------------------------------------
//  win360memoryconfig.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "memory/win360/win360memoryconfig.h"
#include "core/sysfunc.h"

namespace Memory
{
HANDLE volatile Heaps[NumHeapTypes] = { NULL };

//------------------------------------------------------------------------------
/**
*/
void
SetupHeaps()
{
    const SIZE_T megaByte = 1024 * 1024;
    unsigned int i;
    for (i = 0; i < NumHeapTypes; i++)
    {
        n_assert(0 == Heaps[i]);
        SIZE_T initialSize = 0;
        SIZE_T maxSize = 0;
        bool useLowFragHeap = false;
        switch (i)
        {
            case DefaultHeap:
            case ObjectHeap:
            case ObjectArrayHeap:
            case SmallBlockHeap:
                // these heaps start at 8 megabyte and may grow
                initialSize = 4 * megaByte;
                useLowFragHeap = true;
                break;

            case LargeBlockHeap:
            case ResourceHeap:
            case PoolHeap:
                // the large block and resource heaps start at 32 megabyte and may grow(?)
                initialSize = 8 * megaByte;
                break;

            case ScratchHeap:
                // the scratch heap is set to 8 megabyte and may not grow
                initialSize = 8 * megaByte;
                break;

            case StringHeap:
                // the string heap starts at 2 MB and may grow
                initialSize = 2 * megaByte;
                useLowFragHeap = true;
                break;

            case StreamDataHeap:
                // the stream data heap starts at 16 MB and may grow
                initialSize = 16 * megaByte;
                break;

            case Xbox360GraphicsHeap:
            case Xbox360AudioHeap:
                // the special Xbox360 write combined heap, this is handled as a 
                // special case in Memory::Alloc()
                initialSize = 0;
                break;

            default:
                Core::SysFunc::Error("Invalid heap type in Memory::SetupHeaps() (win360memoryconfig.cc)!");
                break;
        }
        if (0 != initialSize)
        {
            Heaps[i] = HeapCreate(HEAP_GENERATE_EXCEPTIONS, initialSize, maxSize);
            if (useLowFragHeap)
            {
                #if __WIN32__
                // enable the Win32 LowFragmentationHeap
                ULONG heapFragValue = 2;
                HeapSetInformation(Heaps[i], HeapCompatibilityInformation, &heapFragValue, sizeof(heapFragValue));
                #endif
            }
        }
        else
        {
            Heaps[i] = 0;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
const char*
GetHeapTypeName(HeapType heapType)
{
    switch (heapType)
    {
        case DefaultHeap:               return "Default Heap";
        case ObjectHeap:                return "Object Heap";
        case ObjectArrayHeap:           return "Object Array Heap";
        case SmallBlockHeap:            return "Small Block Heap";
        case LargeBlockHeap:            return "Large Block Heap";
        case ResourceHeap:              return "Resource Heap";
        case ScratchHeap:               return "Scratch Heap";
        case StringHeap:                return "String Heap";
        case StreamDataHeap:            return "Stream Data Heap";
        case PoolHeap:                  return "Pool Heap";
        case Xbox360GraphicsHeap:       return "Xbox360 Graphics Heap";
        case Xbox360AudioHeap:          return "Xbox360 Audio Heap";
        default:
            Core::SysFunc::Error("Invalid HeapType arg in Memory::GetHeapTypeName()! (win32memoryconfig.cc)");
            return 0;
    }
}

} // namespace Win360
