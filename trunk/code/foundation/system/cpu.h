#pragma once
#ifndef SYSTEM_CPU_H
#define SYSTEM_CPU_H
//------------------------------------------------------------------------------
/**
    @class System::Cpu
    
    Provides information about the system's CPU(s).
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace System
{
class Cpu
{
public:
    /// core id's
    enum CoreId
    {
        Core0 = 0,
        Core1,
        Core2,
        Core3,
        Core4,
        Core5,
        Core6,
        Core7,
        Core8,
        
        MaxNumCores,
        UndefinedCoreId,
    };
};

} // namespace System
//------------------------------------------------------------------------------
#endif
    