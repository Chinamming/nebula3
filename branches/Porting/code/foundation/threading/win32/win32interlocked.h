#pragma once
#ifndef WIN32_WIN32INTERLOCKED_H
#define WIN32_WIN32INTERLOCKED_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32Interlocked
    
    Provides simple atomic operations on shared variables.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32Interlocked
{
public:
    /// interlocked increment
    static void Increment(int volatile& var);
    /// interlocked decrement
    static void Decrement(int volatile& var);
    /// interlocked add
    static void Add(int volatile& var, int add);
};

//------------------------------------------------------------------------------
/**
*/
inline void
Win32Interlocked::Increment(int volatile& var)
{
    InterlockedIncrement((volatile LONG*)&var);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32Interlocked::Decrement(int volatile& var)
{
    InterlockedDecrement((volatile LONG*)&var);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32Interlocked::Add(int volatile& var, int add)
{
    InterlockedExchangeAdd((volatile LONG*)&var, add);
}

} // namespace Win32
//------------------------------------------------------------------------------
#endif