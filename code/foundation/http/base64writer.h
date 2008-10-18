#pragma once
#ifndef HTTP_BASE64WRITER_H
#define HTTP_BASE64WRITER_H
//------------------------------------------------------------------------------
/**
    @class Http::Base64Writer
    
    StreamWriter which implements a Base64 encoder. Encodes blocks of 3 bytes
    with binary data into blocks of 4 bytes of Base64 data.
    
    (C) 2007 Radon Labs GmbH
*/
#include "io/streamwriter.h"

//------------------------------------------------------------------------------
namespace Http
{
class Base64Writer : public IO::StreamWriter
{
    __DeclareClass(Base64Writer);
public:
    /// constructor
    Base64Writer();
    /// destructor
    virtual ~Base64Writer();
    /// write binary data to stream and apply Base64 encoding (all data must be written at once)
    void Write(void* data, SizeT numBytes);

private:
    /// encode a block of up to 3 bytes into a Base64-encoded block of 4 bytes
    void EncodeBlock(uchar* src, uchar* dst, SizeT numBytes);

    SizeT numBytesWritten;
};

} // namespace Http
//------------------------------------------------------------------------------
#endif
    