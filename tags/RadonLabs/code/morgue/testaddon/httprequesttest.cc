//------------------------------------------------------------------------------
//  httprequesttest.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "test/httprequesttest.h"
#include "net/httprequest.h"
#include "io/server.h"
#include "io/textreader.h"
#include "io/filestream.h"

namespace Test
{
ImplementClass(Test::HttpRequestTest, 'HRQT', Test::TestCase);

using namespace Util;
using namespace Net;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
void
HttpRequestTest::Run()
{
    Ptr<IO::Server> ioServer = IO::Server::Create();

    // create a HTTP request object
    Ptr<HttpRequest> httpRequest = HttpRequest::Create();
    httpRequest->SetURI("http://www.radonlabs.de/images/background.jpg");
    httpRequest->SetMethod(HttpRequest::Get);
    if (httpRequest->Send())
    {
        // read resonse...
        Stream* response = httpRequest->GetResponse();
        this->Verify(0 != response);
        this->Verify(response->GetSize() > 0);

        // read response and copy to temp
        response->SetAccessMode(Stream::ReadAccess);
        response->Open();
        void* ptr = response->Map();

        Ptr<FileStream> fileStream = FileStream::Create();
        fileStream->SetURI("temp:httprequesttest.jpg");
        fileStream->SetAccessMode(Stream::WriteAccess);
        fileStream->Open();
        fileStream->Write(ptr, response->GetSize());
        fileStream->Close();

        response->Unmap();
        response->Close();

        this->Verify(IO::Server::Instance()->FileExists("temp:httprequesttest.jpg"));
    }
}

}