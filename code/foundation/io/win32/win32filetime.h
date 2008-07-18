#pragma once
#ifndef WIN32_WIN32FILETIME_H
#define WIN32_WIN32FILETIME_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32FileTime
    
    Implements a Win32-specific file-access time stamp.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32FileTime
{
public:
    /// constructor
    Win32FileTime();
    /// operator ==
    friend bool operator==(const Win32FileTime& a, const Win32FileTime& b);
    /// operator !=
    friend bool operator!=(const Win32FileTime& a, const Win32FileTime& b);
    /// operator >
    friend bool operator>(const Win32FileTime& a, const Win32FileTime& b);
    /// operator <
    friend bool operator<(const Win32FileTime& a, const Win32FileTime& b);

private:
    friend class Win32FSWrapper;
    friend class Win32CalendarTime;

    FILETIME time;
};

//------------------------------------------------------------------------------
/**
*/
inline
Win32FileTime::Win32FileTime()
{
    time.dwLowDateTime = 0;
    time.dwHighDateTime = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
operator==(const Win32FileTime& a, const Win32FileTime& b)
{
    return (0 == CompareFileTime(&(a.time), &(b.time)));
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
operator!=(const Win32FileTime& a, const Win32FileTime& b)
{
    return (0 != CompareFileTime(&(a.time), &(b.time)));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
operator>(const Win32FileTime& a, const Win32FileTime& b)
{
    return (1 == CompareFileTime(&(a.time), &(b.time)));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
operator <(const Win32FileTime& a, const Win32FileTime& b)
{
    return (-1 == CompareFileTime(&(a.time), &(b.time)));
}

}; // namespace Win32
//------------------------------------------------------------------------------
#endif

    