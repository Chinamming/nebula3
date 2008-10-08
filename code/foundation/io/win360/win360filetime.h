#pragma once
#ifndef WIN360_WIN360FILETIME_H
#define WIN360_WIN360FILETIME_H
//------------------------------------------------------------------------------
/**
    @class Win360::Win360FileTime
    
    Implements a Win32/Xbox360-specific file-access time stamp.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win360
{
class Win360FileTime
{
public:
    /// constructor
    Win360FileTime();
    /// operator ==
    friend bool operator==(const Win360FileTime& a, const Win360FileTime& b);
    /// operator !=
    friend bool operator!=(const Win360FileTime& a, const Win360FileTime& b);
    /// operator >
    friend bool operator>(const Win360FileTime& a, const Win360FileTime& b);
    /// operator <
    friend bool operator<(const Win360FileTime& a, const Win360FileTime& b);

private:
    friend class Win360FSWrapper;
    friend class Win360CalendarTime;

    FILETIME time;
};

//------------------------------------------------------------------------------
/**
*/
inline
Win360FileTime::Win360FileTime()
{
    time.dwLowDateTime = 0;
    time.dwHighDateTime = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
operator==(const Win360FileTime& a, const Win360FileTime& b)
{
    return (0 == CompareFileTime(&(a.time), &(b.time)));
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
operator!=(const Win360FileTime& a, const Win360FileTime& b)
{
    return (0 != CompareFileTime(&(a.time), &(b.time)));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
operator>(const Win360FileTime& a, const Win360FileTime& b)
{
    return (1 == CompareFileTime(&(a.time), &(b.time)));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
operator <(const Win360FileTime& a, const Win360FileTime& b)
{
    return (-1 == CompareFileTime(&(a.time), &(b.time)));
}

}; // namespace Win360
//------------------------------------------------------------------------------
#endif

    