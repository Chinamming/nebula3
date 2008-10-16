//------------------------------------------------------------------------------
//  iointerface.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/base/iointerfacehandlerbase.h"
#include "io/filestream.h"

namespace Base
{
__ImplementClass(Base::IOInterfaceHandlerBase, 'IIOH', Messaging::Handler);

using namespace IO;
using namespace Messaging;
using namespace Interface;

//------------------------------------------------------------------------------
/**    
*/
IOInterfaceHandlerBase::IOInterfaceHandlerBase()
{
}

//------------------------------------------------------------------------------
/**    
*/
IOInterfaceHandlerBase::~IOInterfaceHandlerBase()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
    Opens the Interface message handler which does all the interesting stuff.
    This method already runs in the handler thread.
    The method initializes a minimal thread local Nebula3 runtime, just
    enough to handle the IO messages.
*/
void
IOInterfaceHandlerBase::Open()
{
    Handler::Open();
    this->console = Console::Create();
    this->console->Open();
    this->ioServer = IO::IoServer::Create();
    this->ioServer->RegisterStandardUriSchemes();
    this->ioServer->MountStandardZipArchives();
}

//------------------------------------------------------------------------------
/**
    Closes the Interface message handler. This will shut down the
    minimal Nebula3 runtime, the method runs in the handler thread and is
    called just before the thread shuts down.
*/
void
IOInterfaceHandlerBase::Close()
{
    this->ioServer = 0;
    this->console = 0;
    Handler::Close();
}

//------------------------------------------------------------------------------
/**
    Handles incoming messages. This method runs in the handler thread.
*/
bool
IOInterfaceHandlerBase::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(MountZipArchive::Id))
    {
        this->OnMountZipArchive(msg.downcast<Interface::MountZipArchive>());
    }
    else if (msg->CheckId(CreateDirectory::Id))
    {
        this->OnCreateDirectory(msg.downcast<Interface::CreateDirectory>());
    }
    else if (msg->CheckId(DeleteDirectory::Id))
    {
        this->OnDeleteDirectory(msg.downcast<Interface::DeleteDirectory>());
    }
    else if (msg->CheckId(DeleteFile::Id))
    {
        this->OnDeleteFile(msg.downcast<Interface::DeleteFile>());
    }
    else if (msg->CheckId(WriteStream::Id))
    {
        this->OnWriteStream(msg.downcast<Interface::WriteStream>());
    }
    else if (msg->CheckId(ReadStream::Id))
    {
        this->OnReadStream(msg.downcast<Interface::ReadStream>());
    }
    else if (msg->CheckId(CopyFile::Id))
    {
        this->OnCopyFile(msg.downcast<Interface::CopyFile>());
    }
    else
    {
        // unknown message
        return false;        
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
IOInterfaceHandlerBase::OnMountZipArchive(const Ptr<Interface::MountZipArchive>& msg)
{
    n_printf("IOInterface: MountZipArchive %s\n", msg->GetURI().AsString().AsCharPtr());
    IO::IoServer::Instance()->MountZipArchive(msg->GetURI());
}

//------------------------------------------------------------------------------
/**
*/
void
IOInterfaceHandlerBase::OnCreateDirectory(const Ptr<Interface::CreateDirectory>& msg)
{
    n_printf("IOInterface: CreateDirectory %s\n", msg->GetURI().AsString().AsCharPtr());
    msg->SetResult(IO::IoServer::Instance()->CreateDirectory(msg->GetURI()));
}

//------------------------------------------------------------------------------
/**
*/
void
IOInterfaceHandlerBase::OnDeleteDirectory(const Ptr<Interface::DeleteDirectory>& msg)
{
    n_printf("IOInterface: DeleteDirectory %s\n", msg->GetURI().AsString().AsCharPtr());
    msg->SetResult(IO::IoServer::Instance()->DeleteDirectory(msg->GetURI()));
}

//------------------------------------------------------------------------------
/**
*/
void
IOInterfaceHandlerBase::OnCopyFile(const Ptr<Interface::CopyFile>& msg)
{
    n_printf("IOInterface: CopyDirectory %s %s\n", msg->GetFromURI().AsString().AsCharPtr(), msg->GetToURI().AsString().AsCharPtr());
    msg->SetResult(IO::IoServer::Instance()->CopyFile(msg->GetFromURI(), msg->GetToURI()));
}

//------------------------------------------------------------------------------
/**
*/
void
IOInterfaceHandlerBase::OnDeleteFile(const Ptr<Interface::DeleteFile>& msg)
{
    n_printf("IOInterface: DeleteFile %s\n", msg->GetURI().AsString().AsCharPtr());
    msg->SetResult(IO::IoServer::Instance()->DeleteFile(msg->GetURI()));
}

//------------------------------------------------------------------------------
/**
*/
void
IOInterfaceHandlerBase::OnWriteStream(const Ptr<Interface::WriteStream>& msg)
{
    n_printf("IOInterface: WriteStream %s\n", msg->GetURI().AsString().AsCharPtr());

    // create a destination file stream object
    msg->SetResult(false);
    Ptr<Stream> dstStream = IO::IoServer::Instance()->CreateStream(msg->GetURI());
    dstStream->SetAccessMode(Stream::WriteAccess);
    if (dstStream->Open())
    {
        /// @todo handle non-mappable stream
        const Ptr<Stream>& srcStream = msg->GetStream();
        n_assert(srcStream.isvalid());
        n_assert(srcStream->CanBeMapped());
        srcStream->SetAccessMode(Stream::ReadAccess);
        if (srcStream->Open())
        {
            void* ptr = srcStream->Map();
            dstStream->Write(ptr, srcStream->GetSize());
            srcStream->Unmap();
            srcStream->Close();
            msg->SetResult(true);
        }
        dstStream->Close();        
    }
}

//------------------------------------------------------------------------------
/**
*/
void
IOInterfaceHandlerBase::OnReadStream(const Ptr<Interface::ReadStream>& msg)
{
    n_printf("IOInterface: ReadStream %s\n", msg->GetURI().AsString().AsCharPtr());

    // create a file stream which reads in the data from disc
    msg->SetResult(false);
    Ptr<Stream> srcStream = IO::IoServer::Instance()->CreateStream(msg->GetURI());
    srcStream->SetAccessMode(Stream::ReadAccess);
    if (srcStream->Open())
    {
        /// @todo handle non-mappable stream!
        const Ptr<Stream>& dstStream = msg->GetStream();
        n_assert(dstStream.isvalid());
        n_assert(dstStream->CanBeMapped());
        dstStream->SetAccessMode(Stream::WriteAccess);
        Stream::Size srcSize = srcStream->GetSize();
        n_assert(srcSize > 0);
        dstStream->SetSize(srcSize);
        if (dstStream->Open())
        {
            void* ptr = dstStream->Map();
            n_assert(0 != ptr);
            srcStream->Read(ptr, srcSize);
            dstStream->Unmap();
            dstStream->Close();
            msg->SetResult(true);
        }
        srcStream->Close();
    }
}

} // namespace IO