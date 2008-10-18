#pragma once
#ifndef WIN360_WIN360THREADID_H
#define WIN360_WIN360THREADID_H
//------------------------------------------------------------------------------
/**
    @class Win360::Win360ThreadId
    
    A thread id uniquely identifies a thread within the process.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"

namespace Threading
{
typedef DWORD ThreadId;
static const ThreadId InvalidThreadId = 0xffffffff;
}
//------------------------------------------------------------------------------
#endif
