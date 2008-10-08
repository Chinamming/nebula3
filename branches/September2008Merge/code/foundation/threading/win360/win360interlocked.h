#pragma once
#ifndef WIN360_WIN360INTERLOCKED_H
#define WIN360_WIN360INTERLOCKED_H
//------------------------------------------------------------------------------
/**
    @class Win360::Win360Interlocked
    
    Provides simple atomic operations on shared variables.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win360
{
class Win360Interlocked
{
public:
    /// interlocked increment
    static int Increment(int volatile& var);
    /// interlocked decrement
    static int Decrement(int volatile& var);
    /// interlocked add
    static void Add(int volatile& var, int add);
};

//------------------------------------------------------------------------------
/**
*/
inline int
Win360Interlocked::Increment(int volatile& var)
{
    return InterlockedIncrement((volatile LONG*)&var);
}

//------------------------------------------------------------------------------
/**
*/
inline int
Win360Interlocked::Decrement(int volatile& var)
{
    return InterlockedDecrement((volatile LONG*)&var);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win360Interlocked::Add(int volatile& var, int add)
{
    InterlockedExchangeAdd((volatile LONG*)&var, add);
}

} // namespace Win360
//------------------------------------------------------------------------------
#endif