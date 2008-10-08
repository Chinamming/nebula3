//------------------------------------------------------------------------------
//  binaryreader.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/binaryreader.h"

namespace IO
{
__ImplementClass(IO::BinaryReader, 'BINR', IO::StreamReader);

using namespace Util;
using namespace System;

//------------------------------------------------------------------------------
/**
*/
BinaryReader::BinaryReader() :
    enableMapping(false),
    isMapped(false),
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
    short val;
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(short)) <= this->mapEnd);        
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        this->mapCursor += sizeof(val);
    }
    else
    {
        this->stream->Read(&val, sizeof(val));
    }
    this->byteOrder.Convert<short>(val);
    return val;
}

//------------------------------------------------------------------------------
/**
*/ 
unsigned short
BinaryReader::ReadUShort()
{
    ushort val;
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(unsigned short)) <= this->mapEnd);
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        this->mapCursor += sizeof(val);
    }
    else
    {
        this->stream->Read(&val, sizeof(val));
    }
    this->byteOrder.Convert<ushort>(val);
    return val;
}

//------------------------------------------------------------------------------
/**
*/
int
BinaryReader::ReadInt()
{
    int val;
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(int)) <= this->mapEnd);
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        this->mapCursor += sizeof(val);
    }
    else
    {
        this->stream->Read(&val, sizeof(val));
    }
    this->byteOrder.Convert<int>(val);
    return val;
}

//------------------------------------------------------------------------------
/**
*/
unsigned int
BinaryReader::ReadUInt()
{
    uint val;
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(unsigned int)) <= this->mapEnd);
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        this->mapCursor += sizeof(val);
    }
    else
    {
        this->stream->Read(&val, sizeof(val));
    }
    this->byteOrder.Convert<uint>(val);
    return val;
}

//------------------------------------------------------------------------------
/**
*/
float
BinaryReader::ReadFloat()
{
    float val;
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(float)) <= this->mapEnd);
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        this->mapCursor += sizeof(val);
    }
    else
    {
        this->stream->Read(&val, sizeof(val));
    }
    this->byteOrder.Convert<float>(val);
    return val;
}

//------------------------------------------------------------------------------
/**
*/
double
BinaryReader::ReadDouble()
{
    double val;
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(double)) <= this->mapEnd);
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        this->mapCursor += sizeof(val);
    }
    else
    {
        this->stream->Read(&val, sizeof(val));
    }
    this->byteOrder.Convert<double>(val);
    return val;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryReader::ReadBool()
{
    bool val;
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(bool)) <= this->mapEnd);
        Memory::Copy(this->mapCursor, &val, sizeof(val));
        this->mapCursor += sizeof(val);
    }
    else
    {
        this->stream->Read(&val, sizeof(val));        
    }
    return val;
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
    Math::float4 val;
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(Math::float4)) <= this->mapEnd);
        Memory::Copy(this->mapCursor, &val, sizeof(val));     
        this->mapCursor += sizeof(val);
    }
    else
    {
        this->stream->Read(&val, sizeof(val));
    }
    this->byteOrder.Convert<Math::float4>(val);
    return val;
}

//------------------------------------------------------------------------------
/**
*/ 
Math::matrix44
BinaryReader::ReadMatrix44()
{
    Math::matrix44 val;
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(Math::matrix44)) <= this->mapEnd);
        Memory::Copy(this->mapCursor, &val, sizeof(val));        
        this->mapCursor += sizeof(val);
    }
    else
    {
        this->stream->Read(&val, sizeof(val));
    }
    this->byteOrder.Convert<Math::matrix44>(val);
    return val;
}

//------------------------------------------------------------------------------
/**
*/ 
void
BinaryReader::ReadRawData(void* ptr, SizeT numBytes)
{
    n_assert((ptr != 0) && (numBytes > 0));
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
}

} // namespace IO
