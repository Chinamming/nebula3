//------------------------------------------------------------------------------
//  writestream.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iomsg/writestream.h"
#include "io/memorystream.h"

namespace Interface
{
ImplementClass(Interface::WriteStream, 'WRST', Interface::IOMessage);
ImplementMsgId(WriteStream);

using namespace IO;

//------------------------------------------------------------------------------
/**
*/
void
WriteStream::Encode(const Ptr<BinaryWriter>& writer)
{
    IOMessage::Encode(writer);

    // write stream contents
    /// @todo handle non-mappable stream
    this->stream->SetAccessMode(Stream::ReadAccess);
    this->stream->Open();
    void* buf = this->stream->Map();
    writer->WriteUInt(this->stream->GetSize());
    writer->GetStream()->Write(buf, this->stream->GetSize());
    this->stream->Unmap();
    this->stream->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
WriteStream::Decode(const Ptr<BinaryReader>& reader)
{
    IOMessage::Decode(reader);

    // setup stream object
    this->stream = MemoryStream::Create();
    this->stream->SetAccessMode(Stream::WriteAccess);
    this->stream->SetSize(reader->ReadUInt());
    this->stream->Open();
    void* buf = this->stream->Map();
    reader->GetStream()->Read(buf, this->stream->GetSize());
    this->stream->Unmap();
    this->stream->Close();
}

} // namespace IO
