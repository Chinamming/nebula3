//------------------------------------------------------------------------------
//  binarywriter.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/binarywriter.h"

namespace IO
{
ImplementClass(IO::BinaryWriter, 'BINW', IO::StreamWriter);

using namespace Util;
using namespace System;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
BinaryWriter::BinaryWriter() :
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
BinaryWriter::~BinaryWriter()
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
BinaryWriter::Open()
{
    if (StreamWriter::Open())
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
BinaryWriter::Close()
{
    StreamWriter::Close();
    this->isMapped = false;
    this->mapCursor = 0;
    this->mapEnd = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteChar(char c)
{
    if (this->isMapped)
    {
        n_assert(this->mapCursor < this->mapEnd);
        *this->mapCursor++ = c;
    }
    else
    {
        this->stream->Write(&c, sizeof(c));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteUChar(unsigned char c)
{
    if (this->isMapped)
    {
        n_assert(this->mapCursor < this->mapEnd);
        *this->mapCursor++ = c;
    }
    else
    {
        this->stream->Write(&c, sizeof(c));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteShort(short s)
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(s)) <= this->mapEnd);
        if (ByteOrder::Host != this->streamByteOrder)
        {
            short val = ByteOrder::ConvertShort(this->streamByteOrder, ByteOrder::Host, s);
            Memory::Copy(&val, this->mapCursor, sizeof(val));
        }
        else
        {
            Memory::Copy(&s, this->mapCursor, sizeof(s));
        }
        this->mapCursor += sizeof(s);
    }
    else
    {
        if (ByteOrder::Host != this->streamByteOrder)
        {
            short val = ByteOrder::ConvertShort(this->streamByteOrder, ByteOrder::Host, s);
            this->stream->Write(&val, sizeof(val));
        }
        else
        {
            this->stream->Write(&s, sizeof(s));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteUShort(unsigned short s)
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(s)) <= this->mapEnd);
        if (ByteOrder::Host != this->streamByteOrder)
        {
            ushort val = ByteOrder::ConvertUShort(this->streamByteOrder, ByteOrder::Host, s);
            Memory::Copy(&val, this->mapCursor, sizeof(val));
        }
        else
        {
            Memory::Copy(&s, this->mapCursor, sizeof(s));
        }
        this->mapCursor += sizeof(s);
    }
    else
    {
        if (ByteOrder::Host != this->streamByteOrder)
        {
            short val = ByteOrder::ConvertUShort(this->streamByteOrder, ByteOrder::Host, s);
            this->stream->Write(&val, sizeof(val));
        }
        else
        {
            this->stream->Write(&s, sizeof(s));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteInt(int i)
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(i)) <= this->mapEnd);
        if (ByteOrder::Host != this->streamByteOrder)
        {
            int val = ByteOrder::ConvertInt(this->streamByteOrder, ByteOrder::Host, i);
            Memory::Copy(&val, this->mapCursor, sizeof(val));
        }
        else
        {
            Memory::Copy(&i, this->mapCursor, sizeof(i));
        }
        this->mapCursor += sizeof(i);
    }
    else
    {
        if (ByteOrder::Host != this->streamByteOrder)
        {
            int val = ByteOrder::ConvertInt(this->streamByteOrder, ByteOrder::Host, i);
            this->stream->Write(&val, sizeof(val));
        }
        else
        {
            this->stream->Write(&i, sizeof(i));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteUInt(unsigned int i)
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(i)) <= this->mapEnd);
        if (ByteOrder::Host != this->streamByteOrder)
        {
            unsigned int val = ByteOrder::ConvertUInt(this->streamByteOrder, ByteOrder::Host, i);
            Memory::Copy(&val, this->mapCursor, sizeof(val));
        }
        else
        {
            Memory::Copy(&i, this->mapCursor, sizeof(i));
        }
        this->mapCursor += sizeof(i);
    }
    else
    {
        if (ByteOrder::Host != this->streamByteOrder)
        {
            int val = ByteOrder::ConvertUInt(this->streamByteOrder, ByteOrder::Host, i);
            this->stream->Write(&val, sizeof(val));
        }
        else
        {
            this->stream->Write(&i, sizeof(i));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteFloat(float f)
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(f)) <= this->mapEnd);
        if (ByteOrder::Host != this->streamByteOrder)
        {
            float val = ByteOrder::ConvertFloat(this->streamByteOrder, ByteOrder::Host, f);
            Memory::Copy(&val, this->mapCursor, sizeof(val));
        }
        else
        {
            Memory::Copy(&f, this->mapCursor, sizeof(f));
        }
        this->mapCursor += sizeof(f);
    }
    else
    {
        if (ByteOrder::Host != this->streamByteOrder)
        {
            float val = ByteOrder::ConvertFloat(this->streamByteOrder, ByteOrder::Host, f);
            this->stream->Write(&val, sizeof(val));
        }
        else
        {
            this->stream->Write(&f, sizeof(f));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteDouble(double d)
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(d)) <= this->mapEnd);
        if (ByteOrder::Host != this->streamByteOrder)
        {
            double val = ByteOrder::ConvertDouble(this->streamByteOrder, ByteOrder::Host, d);
            Memory::Copy(&val, this->mapCursor, sizeof(val));
        }
        else
        {
            Memory::Copy(&d, this->mapCursor, sizeof(d));
        }
        this->mapCursor += sizeof(d);
    }
    else
    {
        if (ByteOrder::Host != this->streamByteOrder)
        {
            double val = ByteOrder::ConvertDouble(this->streamByteOrder, ByteOrder::Host, d);
            this->stream->Write(&val, sizeof(val));
        }
        else
        {
            this->stream->Write(&d, sizeof(d));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteBool(bool b)
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(b)) <= this->mapEnd);
        Memory::Copy(&b, this->mapCursor, sizeof(b));
        this->mapCursor += sizeof(b);
    }
    else
    {
        this->stream->Write(&b, sizeof(b));
    }
}

//------------------------------------------------------------------------------
/**
    NOTE: for strings, first the length will be written into a
    32-bit int, then the string contents without the 0-terminator.
*/
void
BinaryWriter::WriteString(const Util::String& s)
{
    n_assert(s.Length() < (1<<16));
    this->WriteUShort(ushort(s.Length()));
    if (s.Length() > 0)
    {
        if (this->isMapped)
        {
            n_assert((this->mapCursor + s.Length()) <= this->mapEnd);
            Memory::Copy(s.AsCharPtr(), this->mapCursor, s.Length());
            this->mapCursor += s.Length();
        }
        else
        {
            this->stream->Write((void*)s.AsCharPtr(), s.Length());
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteBlob(const Util::Blob& blob)
{
    this->WriteUInt(blob.Size());
    if (this->isMapped)
    {
        n_assert((this->mapCursor + blob.Size()) <= this->mapEnd);
        Memory::Copy(blob.GetPtr(), this->mapCursor, blob.Size());
        this->mapCursor += blob.Size();
    }
    else
    {
        this->stream->Write(blob.GetPtr(), blob.Size());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteGuid(const Util::Guid& guid)
{
    const unsigned char* ptr;
    SizeT size = guid.AsBinary(ptr);
    Util::Blob blob(ptr, size);
    this->WriteBlob(blob);
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteFloat4(const float4& v)
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(v)) <= this->mapEnd);
        if (ByteOrder::Host != this->streamByteOrder)
        {
            float4 val = ByteOrder::ConvertFloat4(this->streamByteOrder, ByteOrder::Host, v);
            Memory::Copy(&val, this->mapCursor, sizeof(val));
        }
        else
        {
            Memory::Copy(&v, this->mapCursor, sizeof(v));
        }
        this->mapCursor += sizeof(v);
    }
    else
    {
        if (ByteOrder::Host != this->streamByteOrder)
        {
            float4 val = ByteOrder::ConvertFloat4(this->streamByteOrder, ByteOrder::Host, v);
            this->stream->Write(&val, sizeof(val));
        }
        else
        {
            this->stream->Write(&v, sizeof(v));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryWriter::WriteMatrix44(const matrix44& m)
{
    if (this->isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        n_assert((this->mapCursor + sizeof(m)) <= this->mapEnd);
        if (ByteOrder::Host != this->streamByteOrder)
        {
            matrix44 val = ByteOrder::ConvertMatrix44(this->streamByteOrder, ByteOrder::Host, m);
            Memory::Copy(&val, this->mapCursor, sizeof(val));
        }
        else
        {
            Memory::Copy(&m, this->mapCursor, sizeof(m));
        }
        this->mapCursor += sizeof(m);
    }
    else
    {
        if (ByteOrder::Host != this->streamByteOrder)
        {
            matrix44 val = ByteOrder::ConvertMatrix44(this->streamByteOrder, ByteOrder::Host, m);
            this->stream->Write(&val, sizeof(val));
        }
        else
        {
            this->stream->Write(&m, sizeof(m));
        }
    }
}

} // namespace IO
