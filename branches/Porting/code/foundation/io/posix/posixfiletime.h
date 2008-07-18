#pragma once
#ifndef POSIX_POSIXFILETIME_H
#define POSIX_POSIXFILETIME_H
//------------------------------------------------------------------------------
/**
    @class Posix::PosixFileTime
    
    Implements a Posix-specific file-access time stamp.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Posix
{
class PosixFileTime
{
public:
    /// constructor
    PosixFileTime();
    /// operator ==
    friend bool operator==(const PosixFileTime& a, const PosixFileTime& b);
    /// operator !=
    friend bool operator!=(const PosixFileTime& a, const PosixFileTime& b);
    /// operator >
    friend bool operator>(const PosixFileTime& a, const PosixFileTime& b);
    /// operator <
    friend bool operator<(const PosixFileTime& a, const PosixFileTime& b);

private:
    friend class PosixFSWrapper;
    friend class PosixCalendarTime;

    timespec time;
};

//------------------------------------------------------------------------------
/**
*/
inline
PosixFileTime::PosixFileTime()
{
    time.tv_sec = 0;
    time.tv_nsec = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
operator==(const PosixFileTime& a, const PosixFileTime& b)
{
    return (a.time.tv_sec == b.time.tv_sec) && (a.time.tv_nsec == b.time.tv_nsec);
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
operator!=(const PosixFileTime& a, const PosixFileTime& b)
{
    return (a.time.tv_sec != b.time.tv_sec) || (a.time.tv_nsec != b.time.tv_nsec);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
operator>(const PosixFileTime& a, const PosixFileTime& b)
{
    if (a.time.tv_sec < b.time.tv_sec) return false;
    if (a.time.tv_sec > b.time.tv_sec) return true;
    if (a.time.tv_nsec > b.time.tv_sec) return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
operator <(const PosixFileTime& a, const PosixFileTime& b)
{
    if (a.time.tv_sec > b.time.tv_sec) return false;
    if (a.time.tv_sec < b.time.tv_sec) return true;
    if (a.time.tv_nsec < b.time.tv_sec) return true;
    return false;
}

}; // namespace Posix
//------------------------------------------------------------------------------
#endif

