//------------------------------------------------------------------------------
//  httpstreamtest.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "test/httpstreamtest.h"
#include "net/httpstream.h"
#include "io/server.h"

namespace Test
{
ImplementClass(Test::HttpStreamTest, 'HTTT', Test::TestCase)

using namespace Util;
using namespace Net;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
void
HttpStreamTest::Run()
{
    Ptr<IO::Server> ioServer = IO::Server::Create();

    // create one HttpConnection for several downloads
    Ptr<HttpConnection> conn = HttpConnection::Create();
    conn->SetURI("http://www.radonlabs.de/");
    this->Verify(conn->Connect());
    this->Verify(conn->IsConnected());

    if (conn->IsConnected())
    {
        // create a http stream object and download several files
        Ptr<HttpStream> stream0 = HttpStream::Create();
        stream0->SetHttpConnection(conn);
        stream0->SetURI("http://www.radonlabs.de/images/background.jpg");
        this->Verify(stream0->Open());
        this->Verify(stream0->IsOpen());
        this->Verify(stream0->GetSize() > 0);
        stream0->Close();
        this->Verify(!stream0->IsOpen());

        Ptr<HttpStream> stream1 = HttpStream::Create();
        stream1->SetHttpConnection(conn);
        stream1->SetURI("http://www.radonlabs.de/images/n2toolkit.jpg");
        this->Verify(stream1->Open());
        this->Verify(stream1->IsOpen());
        this->Verify(stream1->GetSize() > 0);
        stream1->Close();
        this->Verify(!stream1->IsOpen());

        conn->Disconnect();
    }
    this->Verify(!conn->IsConnected());
}

} // namespace Test
