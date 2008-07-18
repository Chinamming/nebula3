#pragma once
#ifndef DARWIN_DARWININTERLOCKED_H
#define DARWIN_DARWININTERLOCKED_H
//------------------------------------------------------------------------------
/**
    @class Darwin::DarwinInterlocked
    
    Provides simple atomic operations on shared variables.
    
    (C) 2008 Bruce Mitchener, Jr.
*/
#include "core/types.h"
#include <libkern/OSAtomic.h>

//------------------------------------------------------------------------------
namespace Darwin
{
class DarwinInterlocked
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
DarwinInterlocked::Increment(int volatile& var)
{
    OSAtomicIncrement32((volatile int32_t*)&var);
}

//------------------------------------------------------------------------------
/**
*/
inline void
DarwinInterlocked::Decrement(int volatile& var)
{
    OSAtomicDecrement32((volatile int32_t*)&var);
}

//------------------------------------------------------------------------------
/**
*/
inline void
DarwinInterlocked::Add(int volatile& var, int add)
{
    OSAtomicAdd32(add, (volatile int32_t*)&var);
}

} // namespace Darwin
//------------------------------------------------------------------------------
#endif
