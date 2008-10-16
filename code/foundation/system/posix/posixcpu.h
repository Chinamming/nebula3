#pragma once
#ifndef POSIX_POSIXCPU_H
#define POSIX_POSIXCPU_H
//------------------------------------------------------------------------------
/**
    @class Posix::PosixCpu
    
    CPU related definitions for the Posix platform.  
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Posix
{
class PosixCpu
{
public:
    typedef int CoreId;
    
    /// core identifiers, under Posix, we basically don't care...
    static const CoreId InvalidCoreId    = 0xffffffff;
    static const CoreId MainThreadCore   = 0;
    static const CoreId IoThreadCore     = 2;
    static const CoreId RenderThreadCore = 1;
    static const CoreId AudioThreadCore  = 3;
    static const CoreId MiscThreadCore   = 4;
};

} // namespace Posix    
//------------------------------------------------------------------------------
#endif
