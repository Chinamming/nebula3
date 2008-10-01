#pragma once
#ifndef THREADING_THREADLOCALPTR_H
#define THREADING_THREADLOCALPTR_H
//------------------------------------------------------------------------------
/**
    @class Threading::ThreadLocalPtr
    
    @todo describe ThreadLocalPtr class
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "threading/win32/win32threadlocalptr.h"
namespace Threading
{
template<typename T> class ThreadLocalPtr : public Win32::WiiThreadLocalPtr<T>
{
public:

	ThreadLocalPtr::ThreadLocalPtr<T>(T* obj) : Win32::WiiThreadLocalPtr<T>(obj) 
	{
	};
	
	void operator=(T* rhs) 
	{
		Win32::WiiThreadLocalPtr<T>::operator=(rhs);
	};		
};
}
#elif __XBOX360__
#include "threading/xbox360/xbox360threadlocalptr.h"
namespace Threading
{
template<typename T> class ThreadLocalPtr : public Xbox360::WiiThreadLocalPtr<T>
{
public:

	ThreadLocalPtr::ThreadLocalPtr<T>(T* obj) : Xbox360::WiiThreadLocalPtr<T>(obj) 
	{
	};
	
	void operator=(T* rhs) 
	{
		Xbox360::WiiThreadLocalPtr<T>::operator=(rhs);
	};		
};
}
#elif __WII__
#include "threading/wii/wiithreadlocalptr.h"
namespace Threading
{
template<typename T> class ThreadLocalPtr : public Wii::WiiThreadLocalPtr<T>
{
public:

	ThreadLocalPtr::ThreadLocalPtr<T>(T* obj) : Wii::WiiThreadLocalPtr<T>(obj) 
	{
	};
	
	void operator=(T* rhs) 
	{
		Wii::WiiThreadLocalPtr<T>::operator=(rhs);
	};		
};
}
#else
#error "Threading::ThreadLocalPtr not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
