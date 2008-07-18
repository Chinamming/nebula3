#pragma once
#ifndef SYSTEM_WIN32_BYTEORDER_H
#define SYSTEM_WIN32_BYTEORDER_H
//------------------------------------------------------------------------------
/**
    @class System::ByteOrder
    
    Provides information and methods to help with byte ordering issues.

    The following byte orders are defined:

    - ByteOrder::LittleEndian:  Intel byte order
    - ByteOrder::BigEndian:     Motorola byte order
    - ByteOrder::Network        network byte order (always big endian)
    - ByteOrder::Host           hardwired to little or big endian depending on host cpu

    NOTE: on console-platforms it is strongly recommended to store binary data
    in console-byte-order during asset processing on the development PC,
    so that the console doesn't waste loading time with byte conversions!
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace System
{
class ByteOrder
{
public:
    // byte orders
    enum Type
    {
        LittleEndian = 0,           // e.g. x86
        BigEndian,                  // e.g. PowerPC        
        Network = BigEndian,        // network byte order

        #if __WIN32__
        Host = LittleEndian,
        #else
        Host = BigEndian,
        #endif
    };

    /// endian-convert short value
    static short ConvertShort(Type fromByteOrder, Type toByteOrder, short val);
    /// endian-convert ushort value
    static ushort ConvertUShort(Type fromByteOrder, Type toByteOrder, ushort val);
    /// endian-convert int value
    static int ConvertInt(Type fromByteOrder, Type toByteOrder, int val);
    /// endian-convert uint value
    static uint ConvertUInt(Type fromByteOrder, Type toByteOrder, uint val);
    /// endian-convert float value
    static float ConvertFloat(Type fromByteOrder, Type toByteOrder, float val);
    /// endian-convert double value
    static double ConvertDouble(Type fromByteOrder, Type toByteOrder, double val);
    /// endian-convert float4 value
    static Math::float4 ConvertFloat4(Type fromByteOrder, Type toByteOrder, const Math::float4& val);
    /// endian convert matrix44 value
    static Math::matrix44 ConvertMatrix44(Type fromByteOrder, Type toByteOrder, const Math::matrix44& val);
};

//------------------------------------------------------------------------------
/**
*/
__forceinline short
ByteOrder::ConvertShort(Type fromByteOrder, Type toByteOrder, short val)
{
    if (fromByteOrder == toByteOrder)
    {
        return val;
    }
    else
    {
        ushort res = _byteswap_ushort(*(ushort*)&val);
        return *(short*)&res;
    }
}

//------------------------------------------------------------------------------
/**
*/
__forceinline ushort
ByteOrder::ConvertUShort(Type fromByteOrder, Type toByteOrder, ushort val)
{
    if (fromByteOrder == toByteOrder)
    {
        return val;
    }
    else
    {
        return _byteswap_ushort(val);
    }
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
ByteOrder::ConvertInt(Type fromByteOrder, Type toByteOrder, int val)
{
    if (fromByteOrder == toByteOrder)
    {
        return val;
    }
    else
    {
        uint res = _byteswap_ulong(*(uint*)&val);
        return *(int*)&res;
    }
}

//------------------------------------------------------------------------------
/**
*/
__forceinline uint
ByteOrder::ConvertUInt(Type fromByteOrder, Type toByteOrder, uint val)
{
    if (fromByteOrder == toByteOrder)
    {
        return val;
    }
    else
    {
        return _byteswap_ulong(val);
    }
}

//------------------------------------------------------------------------------
/**
*/
__forceinline float
ByteOrder::ConvertFloat(Type fromByteOrder, Type toByteOrder, float val)
{
    if (fromByteOrder == toByteOrder)
    {
        return val;
    }
    else
    {
        uint res = _byteswap_ulong(*(uint*)&val);
        return *(float*)&res;
    }
}

//------------------------------------------------------------------------------
/**
*/
__forceinline double
ByteOrder::ConvertDouble(Type fromByteOrder, Type toByteOrder, double val)
{
    if (fromByteOrder == toByteOrder)
    {
        return val;
    }
    else
    {
        unsigned long long res = _byteswap_uint64(*(unsigned long long*)&val);
        return *(double*)&res;
    }
}

//------------------------------------------------------------------------------
/**
*/
__forceinline Math::float4
ByteOrder::ConvertFloat4(Type fromByteOrder, Type toByteOrder, const Math::float4& val)
{
    if (fromByteOrder == toByteOrder)
    {
        return val;
    }
    else
    {
        return Math::float4(ConvertFloat(fromByteOrder, toByteOrder, val.x()),
                            ConvertFloat(fromByteOrder, toByteOrder, val.y()),
                            ConvertFloat(fromByteOrder, toByteOrder, val.z()),
                            ConvertFloat(fromByteOrder, toByteOrder, val.w()));
    }
}

//------------------------------------------------------------------------------
/**
*/
__forceinline Math::matrix44
ByteOrder::ConvertMatrix44(Type fromByteOrder, Type toByteOrder, const Math::matrix44& val)
{
    if (fromByteOrder == toByteOrder)
    {
        return val;
    }
    else
    {
        return Math::matrix44(ConvertFloat4(fromByteOrder, toByteOrder, val.getrow0()),
                              ConvertFloat4(fromByteOrder, toByteOrder, val.getrow1()),
                              ConvertFloat4(fromByteOrder, toByteOrder, val.getrow2()),
                              ConvertFloat4(fromByteOrder, toByteOrder, val.getrow3()));
    }
}

} // namespace System
//------------------------------------------------------------------------------
#endif
    
