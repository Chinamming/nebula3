#pragma once
#ifndef SYSTEM_BYTEORDER_H
#define SYSTEM_BYTEORDER_H
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

    /// default constructor
    ByteOrder();
    /// constructor: set byte order conversion rule
    ByteOrder(Type fromByteOrder, Type toByteOrder);
    /// set from-byte-order
    void SetFromByteOrder(Type fromByteOrder);
    /// get from-byte-order
    Type GetFromByteOrder() const;
    /// set to-byte-order
    void SetToByteOrder(Type toByteOrder);
    /// get to-byte-order
    Type GetToByteOrder() const;
    /// endian-convert in place
    template<class TYPE> void Convert(TYPE& val) const;

private:
    Type from;
    Type to;
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
ByteOrder::ByteOrder() :
    from(Host),
    to(Host)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
ByteOrder::ByteOrder(ByteOrder::Type fromByteOrder, ByteOrder::Type toByteOrder) :
    from(fromByteOrder),
    to(toByteOrder)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
ByteOrder::SetFromByteOrder(Type fromByteOrder)
{
    this->from = fromByteOrder;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline ByteOrder::Type
ByteOrder::GetFromByteOrder() const
{
    return this->from;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
ByteOrder::SetToByteOrder(Type toByteOrder)
{
    this->to = toByteOrder;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline ByteOrder::Type
ByteOrder::GetToByteOrder() const
{
    return this->to;
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
ByteOrder::Convert<short>(short& val) const
{
    if (this->from != this->to)
    {
        ushort res = _byteswap_ushort(*(ushort*)&val);
        val = *(short*)&res;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
ByteOrder::Convert<ushort>(ushort& val) const
{
    if (this->from != this->to)
    {
        val = _byteswap_ushort(val);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
ByteOrder::Convert<int>(int& val) const
{
    if (this->from != this->to)
    {
        uint res = _byteswap_ulong(*(uint*)&val);
        val = *(int*)&res;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
ByteOrder::Convert<uint>(uint& val) const
{
    if (this->from != this->to)
    {
        val = _byteswap_ulong(val);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
ByteOrder::Convert<float>(float& val) const
{
    if (this->from != this->to)
    {
        uint res = _byteswap_ulong(*(uint*)&val);
        val = *(float*)&res;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
ByteOrder::Convert<double>(double& val) const
{
    if (this->from != this->to)
    {
        unsigned long long res = _byteswap_uint64(*(unsigned long long*)&val);
        val = *(double*)&res;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
ByteOrder::Convert<Math::float4>(Math::float4& val) const
{
    if (this->from != this->to)
    {
        Convert<float>(val.x());
        Convert<float>(val.y());
        Convert<float>(val.z());
        Convert<float>(val.w());
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
ByteOrder::Convert<Math::matrix44>(Math::matrix44& val) const
{
    if (this->from != this->to)
    {
        Math::float4 row0 = val.getrow0();
        Math::float4 row1 = val.getrow1();
        Math::float4 row2 = val.getrow2();
        Math::float4 row3 = val.getrow3();
        Convert<Math::float4>(row0);
        Convert<Math::float4>(row1);
        Convert<Math::float4>(row2);
        Convert<Math::float4>(row3);
        val.setrow0(row0);
        val.setrow1(row1);
        val.setrow2(row2);
        val.setrow3(row3);
    }
}

} // namespace System
//------------------------------------------------------------------------------
#endif
    