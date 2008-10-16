#pragma once
#ifndef THREADING_THREADID_H
#define THREADING_THREADID_H
//------------------------------------------------------------------------------
/**
    @type Threading::ThreadId
    
    The ThreadId type is used to uniqely identify a Nebula3 Thread. The
    main thread always has a ThreadId of 0. Get the current thread-id
    with the static method Thread::GetMyThreadId().
    
    (C) 2008 Radon Labs GmbH
*/
#if (__WIN32__ || __XBOX360__)
#include "threading/win360/win360threadid.h"
#else
#error "Threading::ThreadId not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif

