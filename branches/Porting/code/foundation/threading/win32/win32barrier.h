#pragma once
#ifndef WIN32_WIN32BARRIER_H
#define WIN32_WIN32BARRIER_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32Barrier
    
    Implements the 2 macros ReadWriteBarrier and MemoryBarrier.
    
    ReadWriteBarrier prevents the compiler from re-ordering memory
    accesses accross the barrier.

    MemoryBarrier prevents the CPU from reordering memory access across
    the barrier (all memory access will be finished before the barrier
    is crossed).
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
#define ReadWriteBarrier _ReadWriterBarrier
// NOTE: MemoryBarrier is defined by windows.h
//------------------------------------------------------------------------------
#endif
