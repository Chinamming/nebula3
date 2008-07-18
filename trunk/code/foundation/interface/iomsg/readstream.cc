//------------------------------------------------------------------------------
//  readstream.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iomsg/readstream.h"
#include "io/memorystream.h"

namespace Interface
{
ImplementClass(Interface::ReadStream, 'RRST', Interface::IOMessage);
ImplementMsgId(ReadStream);

using namespace IO;

//------------------------------------------------------------------------------
/**
*/
void
ReadStream::Encode(const Ptr<BinaryWriter>& writer)
{
    IOMessage::Encode(writer);

    // write stream contents
    this->stream->SetAccessMode(Stream::ReadAccess);
    this->stream->Open();
    void* buf = this->stream->Map();
    writer->WriteInt(this->stream->GetSize());
    writer->GetStream()->Write(buf, this->stream->GetSize());
    this->stream->Unmap();
    this->stream->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
ReadStream::Decode(const Ptr<BinaryReader>& reader)
{
    IOMessage::Decode(reader);

    // read stream contents
    this->stream = MemoryStream::Create();
    this->stream->SetAccessMode(Stream::WriteAccess);
    this->stream->SetSize(reader->ReadInt());
    this->stream->Open();
    void* buf = this->stream->Map();
    reader->GetStream()->Read(buf, this->stream->GetSize());
    this->stream->Unmap();
    this->stream->Close();
}

} // namespace IO
