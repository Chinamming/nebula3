#pragma once
#ifndef INTERFACE_WRITESTREAM_H
#define INTERFACE_WRITESTREAM_H
//------------------------------------------------------------------------------
/**
    @class Interface::WriteStream
    
    IOInterface message to asynchronously write a stream.
    
    (C) 2006 Radon Labs GmbH
*/
#include "interface/iomsg/iomessage.h"
#include "io/stream.h"

//------------------------------------------------------------------------------
namespace Interface
{
class WriteStream : public IOMessage
{
    DeclareClass(WriteStream);
    DeclareMsgId;
public:
    /// set stream which provides the data to write
    void SetStream(const Ptr<IO::Stream>& stream);
    /// get stream which provides the data
    const Ptr<IO::Stream>& GetStream() const;
    /// encode message into a stream
    void Encode(const Ptr<IO::BinaryWriter>& writer);
    /// decode message from a stream
    void Decode(const Ptr<IO::BinaryReader>& reader);

private:
    Ptr<IO::Stream> stream;
};

//------------------------------------------------------------------------------
/**
    NOTE: We don't make a copy of the stream. Streams are
    multithreading-safe in a way that only one thread may open a stream
    at any time. If this introduces multithreading issues it's safest
    to make a copy of the stream.
*/
inline void
WriteStream::SetStream(const Ptr<IO::Stream>& s)
{
    this->stream = s;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<IO::Stream>&
WriteStream::GetStream() const
{
    return this->stream;
}

} // namespace Interface
//------------------------------------------------------------------------------
#endif

