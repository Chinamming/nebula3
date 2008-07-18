//------------------------------------------------------------------------------
//  binaryreader.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/binaryreader.h"

namespace IO
{
ImplementClass(IO::BinaryReader, 'BINR', IO::StreamReader);

using namespace Util;
using namespace System;

//------------------------------------------------------------------------------
/**
*/
BinaryReader::BinaryReader() :
    enableMapping(false),
    isMapped(false),
    streamByteOrder(ByteOrder::Host),
    mapCursor(0),
    mapEnd(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
BinaryReader::~BinaryReader()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryReader::Open()
{
    if (StreamReader::Open())
    {
        if (this->enableMapping && this->stream->CanBeMapped())
        {
            this->isMapped = true;
            this->mapCursor = (unsigned char*) this->stream->Map();
            this->mapEnd = this->mapCursor + this->stream->GetSize();
        }
        else
        {
            this->isMapped = false;
            this->mapCursor = 0;
            this->mapEnd = 0;
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryReader::Close()
{
    StreamReader::Close();
    this->isMapped = false;
    this->mapCursor = 0;
    this->mapEnd = 0;
}

//------------------------------------------------------------------------------
/**
*/
char
BinaryReader::ReadChar()
{
    if (this->isMapped)
    {
        n_assert(this->mapCursor < this->mapEnd);
        return *this->mapCursor++;
    }
    else
    {
        char c;
        this->stream->Read(&c, sizeof(c));
        return c;
    }
}

//------------------------------------------------------------------------------
/**
*/
unsigned char
BinaryReader::ReadUChar()
{
    if (this->isMapped)
    {
        n_assert(this->mapCursor < this->mapEnd);
        return *this->mapCursor++;
    }
    else
    {
        unsigned char c;
        this->stream->Read(&c, sizeof(c));
        return c;
    }
}

//------------------------------------------------------------------------------
/**
*/
short
BinaryReader::ReadShort()
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(short)) <= this->mapEnd);        
        short val;
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertShort(this->streamByteOrder, ByteOrder::Host, val);
        }
        this->mapCursor += sizeof(val);
        return val;
    }
    else
    {
        short val;
        this->stream->Read(&val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertShort(this->streamByteOrder, ByteOrder::Host, val);
        }
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/ 
unsigned short
BinaryReader::ReadUShort()
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(unsigned short)) <= this->mapEnd);
        unsigned short val;
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertUShort(this->streamByteOrder, ByteOrder::Host, val);
        }
        this->mapCursor += sizeof(val);
        return val;
    }
    else
    {
        unsigned short val;
        this->stream->Read(&val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertUShort(this->streamByteOrder, ByteOrder::Host, val);
        }
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
int
BinaryReader::ReadInt()
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(int)) <= this->mapEnd);
        int val;
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertInt(this->streamByteOrder, ByteOrder::Host, val);
        }
        this->mapCursor += sizeof(val);
        return val;
    }
    else
    {
        int val;
        this->stream->Read(&val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertInt(this->streamByteOrder, ByteOrder::Host, val);
        }
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
unsigned int
BinaryReader::ReadUInt()
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(unsigned int)) <= this->mapEnd);
        unsigned int val;
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertUInt(this->streamByteOrder, ByteOrder::Host, val);
        }
        this->mapCursor += sizeof(val);
        return val;
    }
    else
    {
        unsigned int val;
        this->stream->Read(&val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertUInt(this->streamByteOrder, ByteOrder::Host, val);
        }
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
float
BinaryReader::ReadFloat()
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(float)) <= this->mapEnd);
        float val;
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertFloat(this->streamByteOrder, ByteOrder::Host, val);
        }
        this->mapCursor += sizeof(val);
        return val;
    }
    else
    {
        float val;
        this->stream->Read(&val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertFloat(this->streamByteOrder, ByteOrder::Host, val);
        }
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
double
BinaryReader::ReadDouble()
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(double)) <= this->mapEnd);
        double val;
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertDouble(this->streamByteOrder, ByteOrder::Host, val);
        }
        this->mapCursor += sizeof(val);
        return val;
    }
    else
    {
        double val;
        this->stream->Read(&val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertDouble(this->streamByteOrder, ByteOrder::Host, val);
        }
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryReader::ReadBool()
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(bool)) <= this->mapEnd);
        bool val;
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        this->mapCursor += sizeof(val);
        return val;
    }
    else
    {
        bool b;
        this->stream->Read(&b, sizeof(b));
        return b;
    }
}

//------------------------------------------------------------------------------
/**
*/
Util::String
BinaryReader::ReadString()
{
    if (this->isMapped)
    {
        ushort length = this->ReadUShort();
        n_assert((this->mapCursor + length) <= this->mapEnd);
        Util::String str;
        if (length > 0)
        {
            str.Reserve(length + 1);
            char* buf = (char*) str.AsCharPtr();
            Memory::Copy(this->mapCursor, buf, length);
            this->mapCursor += length;
            buf[length] = 0;
        }
        return str;    
    }
    else
    {
        ushort length = this->ReadUShort();
        Util::String str;
        if (length > 0)
        {
            str.Reserve(length + 1);
            char* buf = (char*) str.AsCharPtr();
            this->stream->Read((void*)buf, length);
            buf[length] = 0;
        }
        return str;
    }
}

//------------------------------------------------------------------------------
/**
*/
Util::Blob
BinaryReader::ReadBlob()
{
    SizeT numBytes = this->ReadUInt();
    Util::Blob blob(numBytes);
    void* ptr = const_cast<void*>(blob.GetPtr());
    if (this->isMapped)
    {
        n_assert((this->mapCursor + numBytes) <= this->mapEnd);
        Memory::Copy(this->mapCursor, ptr, numBytes);
        this->mapCursor += numBytes;
    }
    else
    {
        this->stream->Read(ptr, numBytes);
    }
    return blob;
}

//------------------------------------------------------------------------------
/**
*/
Util::Guid
BinaryReader::ReadGuid()
{
    Util::Blob blob = this->ReadBlob();
    return Util::Guid((const unsigned char*) blob.GetPtr(), blob.Size());
}

//------------------------------------------------------------------------------
/**
*/ 
Math::float4
BinaryReader::ReadFloat4()
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(Math::float4)) <= this->mapEnd);
        Math::float4 val;
        Memory::Copy(this->mapCursor, &val, sizeof(val));     
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertFloat4(this->streamByteOrder, ByteOrder::Host, val);
        }
        this->mapCursor += sizeof(val);
        return val;
    }
    else
    {
        Math::float4 val;
        this->stream->Read(&val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertFloat4(this->streamByteOrder, ByteOrder::Host, val);
        }
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/ 
Math::matrix44
BinaryReader::ReadMatrix44()
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(Math::matrix44)) <= this->mapEnd);
        Math::matrix44 val;
        Memory::Copy(this->mapCursor, &val, sizeof(val));        
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertMatrix44(this->streamByteOrder, ByteOrder::Host, val);
        }
        this->mapCursor += sizeof(val);
        return val;
    }
    else
    {
        Math::matrix44 val;
        this->stream->Read(&val, sizeof(val));
        if (ByteOrder::Host != this->streamByteOrder)
        {
            val = ByteOrder::ConvertMatrix44(this->streamByteOrder, ByteOrder::Host, val);
        }
        return val;
    }
}

} // namespace IO
