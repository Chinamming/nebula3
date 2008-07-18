#pragma once
#ifndef SYSTEM_PLATFORM_H
#define SYSTEM_PLATFORM_H
//------------------------------------------------------------------------------
/**
    @file system/platform.h

    Platform-specific header

    (C) 2007 Radon Labs GmbH
*/
#include "system/config.h"
#if __WIN32__
#include "system/win32/platform.h"
#elif __XBOX360__
#include "system/xbox360/platform.h"
#elif __WII__
#include "system/wii/platform.h"
#else
#error "platform.h not implemented on this platform"
#endif
//------------------------------------------------------------------------------
#endif
