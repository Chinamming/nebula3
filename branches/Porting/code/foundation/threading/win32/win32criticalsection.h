#pragma once
#ifndef THREADING_WIN32CRITICALSECTION_H
#define THREADING_WIN32CRITICALSECTION_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32CriticalSection
  
    Win32-implementation of critical section. Critical section
    objects are used to protect a portion of code from parallel
    execution. Define a static critical section object and
    use its Enter() and Leave() methods to protect critical sections
    of your code.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32CriticalSection
{
public:
    /// constructor
    Win32CriticalSection();
    /// destructor
    ~Win32CriticalSection();
    /// enter the critical section
    void Enter();
    /// leave the critical section
    void Leave();
private:
    CRITICAL_SECTION criticalSection;
};

//------------------------------------------------------------------------------
/**
*/
inline
Win32CriticalSection::Win32CriticalSection()
{
    InitializeCriticalSectionAndSpinCount(&this->criticalSection, 1024);
}

//------------------------------------------------------------------------------
/**
*/
inline
Win32CriticalSection::~Win32CriticalSection()
{
    DeleteCriticalSection(&this->criticalSection);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Win32CriticalSection::Enter()
{
    EnterCriticalSection(&this->criticalSection);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Win32CriticalSection::Leave()
{
    LeaveCriticalSection(&this->criticalSection);
}

};
//------------------------------------------------------------------------------
#endif