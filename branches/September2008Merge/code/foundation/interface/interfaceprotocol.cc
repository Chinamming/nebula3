//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interfaceprotocol.h"

namespace Interface
{
    ImplementClass(Interface::CopyFile, 'cofi', Messaging::Message);
    ImplementMsgId(CopyFile);
    ImplementClass(Interface::IOMessage, 'iomg', Messaging::Message);
    ImplementMsgId(IOMessage);
    ImplementClass(Interface::CreateDirectory, 'crdi', Messaging::Message);
    ImplementMsgId(CreateDirectory);
    ImplementClass(Interface::DeleteDirectory, 'dedi', Messaging::Message);
    ImplementMsgId(DeleteDirectory);
    ImplementClass(Interface::DeleteFile, 'defi', Messaging::Message);
    ImplementMsgId(DeleteFile);
    ImplementClass(Interface::MountZipArchive, 'mozi', Messaging::Message);
    ImplementMsgId(MountZipArchive);
    ImplementClass(Interface::ReadStream, 'rest', Messaging::Message);
    ImplementMsgId(ReadStream);
    ImplementClass(Interface::WriteStream, 'wrst', Messaging::Message);
    ImplementMsgId(WriteStream);
} // Interface

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
