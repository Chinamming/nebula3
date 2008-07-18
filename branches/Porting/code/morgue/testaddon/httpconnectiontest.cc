//------------------------------------------------------------------------------
//  httpconnectiontest.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "test/httpconnectiontest.h"
#include "net/httpconnection.h"
#include "io/server.h"

namespace Test
{
ImplementClass(Test::HttpConnectionTest, 'HTCT', Test::TestCase);

using namespace Util;
using namespace Net;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
void
HttpConnectionTest::Run()
{
    Ptr<IO::Server> ioServer = IO::Server::Create();

    // create a HTTP connection object
    Ptr<HttpConnection> http = HttpConnection::Create();

    http->SetURI("http://www.google.com");
    this->Verify(http->Connect());
    if (http->IsConnected())
    {
        this->Verify(http->IsConnected());
        http->Disconnect();
        this->Verify(!http->IsConnected());
    }
}

}   