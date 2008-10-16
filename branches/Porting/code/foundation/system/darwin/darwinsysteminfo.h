#pragma once
#ifndef DARWIN_DARWINSYSTEMINFO_H
#define DARWIN_DARWINSYSTEMINFO_H
//------------------------------------------------------------------------------
/**
    @class Darwin::DarwinSystemInfo
    
    Provide information about the system we're running on.
    
    (C) 2008 Radon Labs GmbH
*/
#include "system/base/systeminfobase.h"

//------------------------------------------------------------------------------
namespace Darwin
{
class DarwinSystemInfo : public Base::SystemInfoBase
{
public:
    /// constructor
    DarwinSystemInfo();
};

} // namespace Darwin
//------------------------------------------------------------------------------
#endif    
