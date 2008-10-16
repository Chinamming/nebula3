#pragma once
#ifndef SYSTEM_BYTEORDER_H
#define SYSTEM_BYTEORDER_H

#if __WIN32__
#include "system/win32/win32byteorder.h"
#elif __XBOX360__
#include "system/xbox360/xbox360byteorder.h"
#elif __WII__
#include "system/wii/wiibyteorder.h"
#elif __APPLE__
#include "system/darwin/darwinbyteorder.h"
#else
#error "IMPLEMENT ME!"
#endif

//------------------------------------------------------------------------------
#endif
