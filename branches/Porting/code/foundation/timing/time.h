#pragma once
#ifndef TIMING_TIME_H
#define TIMING_TIME_H
//------------------------------------------------------------------------------
/**
    @file timing/time.h
  
    Typedefs for the Timing subsystem
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Timing
{
/// the time datatype
typedef double Time;

//------------------------------------------------------------------------------
/**
    Put current thread to sleep for specified amount of seconds.
*/
inline void
Sleep(Time t)
{
    n_sleep(t);
}

};
//------------------------------------------------------------------------------
#endif
