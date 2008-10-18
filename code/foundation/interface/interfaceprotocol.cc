//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interfaceprotocol.h"

namespace Interface
{
    __ImplementClass(Interface::CopyFile, 'cofi', Messaging::Message);
    ImplementMsgId(CopyFile);
    __ImplementClass(Interface::IOMessage, 'iomg', Messaging::Message);
    ImplementMsgId(IOMessage);
    __ImplementClass(Interface::CreateDirectory, 'crdi', Messaging::Message);
    ImplementMsgId(CreateDirectory);
    __ImplementClass(Interface::DeleteDirectory, 'dedi', Messaging::Message);
    ImplementMsgId(DeleteDirectory);
    __ImplementClass(Interface::DeleteFile, 'defi', Messaging::Message);
    ImplementMsgId(DeleteFile);
    __ImplementClass(Interface::MountZipArchive, 'mozi', Messaging::Message);
    ImplementMsgId(MountZipArchive);
    __ImplementClass(Interface::ReadStream, 'rest', Messaging::Message);
    ImplementMsgId(ReadStream);
    __ImplementClass(Interface::WriteStream, 'wrst', Messaging::Message);
    ImplementMsgId(WriteStream);
} // Interface

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
