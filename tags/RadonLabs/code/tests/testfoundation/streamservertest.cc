//------------------------------------------------------------------------------
//  streamservertest.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "streamservertest.h"
#include "io/ioserver.h"
#include "io/filestream.h"
#include "io/memorystream.h"

namespace Test
{
ImplementClass(Test::StreamServerTest, 'SSVT', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
void
StreamServerTest::Run()
{
    Ptr<IoServer> ioServer = IoServer::Create();

    // register some classes with the stream server
    ioServer->RegisterUriScheme("file", FileStream::RTTI);
    ioServer->RegisterUriScheme("memory", MemoryStream::RTTI);

    this->Verify(ioServer->IsUriSchemeRegistered("file"));
    this->Verify(ioServer->IsUriSchemeRegistered("memory"));
    this->Verify(!ioServer->IsUriSchemeRegistered("ftp"));
    this->Verify(&ioServer->GetStreamClassByUriScheme("file") == &FileStream::RTTI);
    this->Verify(&ioServer->GetStreamClassByUriScheme("memory") == &MemoryStream::RTTI);

    URI absFileURI("file:///c:/temp");
    URI relFileURI("temp/bla.txt");
    URI memURI("memory:///test");
    Ptr<Stream> absFileStream = ioServer->CreateStream(absFileURI);
    Ptr<Stream> relFileStream = ioServer->CreateStream(relFileURI);
    Ptr<Stream> memStream = ioServer->CreateStream(memURI);
    this->Verify(absFileStream->IsInstanceOf(FileStream::RTTI));
    this->Verify(relFileStream->IsInstanceOf(FileStream::RTTI));
    this->Verify(memStream->IsInstanceOf(MemoryStream::RTTI));
    this->Verify(absFileStream->GetURI() == absFileURI);
    this->Verify(relFileStream->GetURI() == relFileURI);
    this->Verify(memStream->GetURI() == memURI);
}

} // namespace Test