//------------------------------------------------------------------------------
//  copyfile.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iomsg/copyfile.h"

namespace Interface
{
ImplementClass(Interface::CopyFile, 'CFIL', Messaging::Message);
ImplementMsgId(CopyFile);

using namespace IO;

//------------------------------------------------------------------------------
/**
*/
void
CopyFile::Encode(const Ptr<BinaryWriter>& writer)
{
    Message::Encode(writer);
    writer->WriteString(this->from.AsString());
    writer->WriteString(this->to.AsString());
}

//------------------------------------------------------------------------------
/**
*/
void
CopyFile::Decode(const Ptr<BinaryReader>& reader)
{
    Message::Decode(reader);
    this->from = reader->ReadString();
    this->to = reader->ReadString();
}

} // namespace Interface