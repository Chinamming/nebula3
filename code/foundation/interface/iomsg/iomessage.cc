//------------------------------------------------------------------------------
//  iomessage.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iomsg/iomessage.h"

namespace Interface
{
ImplementClass(Interface::IOMessage, 'IOMG', Messaging::Message);
ImplementMsgId(IOMessage);

using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IOMessage::IOMessage() :
    result(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
IOMessage::Encode(const Ptr<BinaryWriter>& writer)
{
    Message::Encode(writer);
    writer->WriteString(this->uri.AsString());
    writer->WriteBool(this->result);
}

//------------------------------------------------------------------------------
/**
*/
void
IOMessage::Decode(const Ptr<BinaryReader>& reader)
{
    Message::Decode(reader);
    this->uri = reader->ReadString();
    this->result = reader->ReadBool();
}

} // namespace Interface
