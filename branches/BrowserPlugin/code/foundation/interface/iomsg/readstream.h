#pragma once
#ifndef INTERFACE_READSTREAM_H
#define INTERFACE_READSTREAM_H
//------------------------------------------------------------------------------
/**
    @class Interface::ReadStream
    
    IOInterface message to asynchronously read a stream. Note that the
    caller must provide a mappable stream which consumes the read
    content.
        
    (C) 2006 Radon Labs GmbH
*/
#include "interface/iomsg/iomessage.h"
#include "io/stream.h"

//------------------------------------------------------------------------------
namespace Interface
{
class ReadStream : public IOMessage
{
    DeclareClass(ReadStream);
    DeclareMsgId;
public:
    /// set a stream which consumes the result of the read operation
    void SetStream(const Ptr<IO::Stream>& stream);
    /// get stream with read result (after message has been handled!)
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
    Set the caller-provided stream which will be filled with
    the read data. Note that the contents of the stream will only
    be valid after the message has been handled by IOInterface.
*/
inline void
ReadStream::SetStream(const Ptr<IO::Stream>& s)
{
    this->stream = s;
}

//------------------------------------------------------------------------------
/**
    Get the stream which will contain the read data (after the
    message has been handled).
*/
inline const Ptr<IO::Stream>&
ReadStream::GetStream() const
{
    return this->stream;
}

} // namespace Interface
//------------------------------------------------------------------------------
#endif
