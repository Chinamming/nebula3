#pragma once
#ifndef POSIX_POSIXINTERLOCKED_H
#define POSIX_POSIXINTERLOCKED_H
//------------------------------------------------------------------------------
/**
    @class Posix::PosixInterlocked

    Provides simple atomic operations on shared variables.

    (C) 2007 Oleg Khryptul (Haron)
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Posix
{
class PosixInterlocked
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
PosixInterlocked::Increment(int volatile& var)
{
    __sync_fetch_and_add(&var, 1);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
PosixInterlocked::Decrement(int volatile& var)
{
    __sync_fetch_and_sub(&var, 1);
}

//------------------------------------------------------------------------------
/**
*/
inline void
PosixInterlocked::Add(int volatile& var, int add)
{
    __sync_fetch_and_add(&var, add);
}

} // namespace Posix
//------------------------------------------------------------------------------
#endif

