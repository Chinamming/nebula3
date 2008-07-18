//------------------------------------------------------------------------------
//  win32httprequest.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/win32/win32httprequest.h"
#include "io/memorystream.h"

namespace Win32
{
ImplementClass(Win32::Win32HttpRequest, 'htrq', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
Win32HttpRequest::Win32HttpRequest() :
    method(Get),
    httpVersion("HTTP/1.1")
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Win32HttpRequest::~Win32HttpRequest()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Sets the current error to InternetGetLastResponseInfo().
*/
void
Win32HttpRequest::SetErrorToInternetLastResponse()
{
    char buf[1024];
    DWORD bufSize = sizeof(buf);
    DWORD errorCode;
    InternetGetLastResponseInfo(&errorCode, buf, &bufSize);
    this->SetError(buf);
}

//------------------------------------------------------------------------------
/**
    This does all the work. If no external HttpConnection is set, a 
    private HttpConnection object will be created. Likewise, if the
    sent http request produces a response, and no ResponseStream is set,
    a memory stream will be created which consumes the response.
*/
bool
Win32HttpRequest::Send()
{
    // clear any previus response stream
    this->responseStream = 0;

    // first make sure we have a connection
    if (!this->httpConnection.isvalid())
    {
        // no external http connection set, create our own
        this->httpConnection = Net::HttpConnection::Create();
        this->httpConnection->SetURI(this->GetURI());
        if (!this->httpConnection->Connect())
        {
            this->httpConnection = 0;
            return false;
        }
    }
    n_assert(this->httpConnection->IsConnected());

    // now open the request
    HINTERNET hConnect = this->httpConnection->GetConnectionHandle();
    n_assert(0 != hConnect);

    // open a http request
    String method = this->GetMethodAsString();
    HINTERNET hRequest = HttpOpenRequest(hConnect,                         // hConnect
                                         method.AsCharPtr(),               // lpszVerb
                                         this->uri.GetTail().AsCharPtr(),  // lpszObject
                                         this->httpVersion.AsCharPtr(),    // lpszVersion
                                         NULL,                             // lpszReferer
                                         NULL,                             /// @todo set accept types!
                                         0,                                // dwFlags
                                         0);                               // dwContext
    if (0 == hRequest)
    {
        this->SetErrorToInternetLastResponse();
        return false;
    }

    // add optional request headers
    this->SetAdditionalRequestHeaders(hRequest);

    // prepare sending the request with optional content
    LPVOID contentPtr = NULL;
    DWORD contentLength = 0;
    if (this->contentStream.isvalid())
    {
        n_assert(this->contentStream->CanBeMapped());
        this->contentStream->SetAccessMode(Stream::ReadAccess);
        if (this->contentStream->Open())
        {
            contentPtr = this->contentStream->Map();
            n_assert(contentPtr != 0);
            contentLength = this->contentStream->GetSize();
        }
    }

    // send the request
    BOOL success = HttpSendRequest(hRequest,        // hRequest
                                   NULL,            // lpszHeaders
                                   0,               // dwHeadersLength
                                   contentPtr,      // lpOptional
                                   contentLength);  // dwOptionalLength

    // need to unmap the content stream
    if (this->contentStream.isvalid())
    {
        this->contentStream->Unmap();
        this->contentStream->Close();
        contentPtr = 0;
    }

    // check for error
    if (!success)
    {
        this->SetErrorToInternetLastResponse();
        InternetCloseHandle(hRequest);
        return false;
    }

    /// @todo extract cookie information!

    // read back response
    if (!this->ReadResponse(hRequest))
    {
        InternetCloseHandle(hRequest);
    }

    // cleanup and exit
    InternetCloseHandle(hRequest);
    return true;
}

//------------------------------------------------------------------------------
/**
    This concatenates the additionally defined request headers into
    a string and sets it on the provided request handle.
*/
void
Win32HttpRequest::SetAdditionalRequestHeaders(HINTERNET hRequest)
{
    if (this->requestHeaders.Size() > 0)
    {
        String str;
        str.Reserve(256);
        IndexT i;
        for (i = 0; i < this->requestHeaders.Size(); i++)
        {
            const String& key = this->requestHeaders.KeyAtIndex(i);
            const String& val = this->requestHeaders.ValueAtIndex(i);
            str.Append(key);
            str.Append(": ");
            str.Append(val);
            str.Append("\r\n");
        }
        BOOL success = HttpAddRequestHeaders(hRequest,          // hConnect
                                             str.AsCharPtr(),   // lpszHeaders
                                             (DWORD)-1L,               // dwHeadersLength (-1 -> 0 terminated)
                                             HTTP_ADDREQ_FLAG_ADD); // dwModifiers (@todo do we want more control here?)
        n_assert2(success, "Adding request headers failed!");
    }
}

//------------------------------------------------------------------------------
/**
    This reads back the response from the HTTP server into a memory
    stream. The stream can be decoded using a HttpResponseReader stream
    reader object.

    @todo Hmm, this method cannot fail at the moment?
*/
bool
Win32HttpRequest::ReadResponse(HINTERNET hRequest)
{
    // create a memory stream which holds the data
    this->responseStream = IO::MemoryStream::Create();
    this->responseStream->SetAccessMode(Stream::WriteAccess);
    bool success = this->responseStream->Open();
    n_assert(success);

    DWORD numBytesToRead = 0;
    BOOL readSuccess = TRUE;
    while (readSuccess && 
           InternetQueryDataAvailable(hRequest, &numBytesToRead, 0, 0) && 
           (numBytesToRead > 0))
    {
        char* buffer = (char*) Memory::Alloc(numBytesToRead);
        char* ptr = buffer;
        DWORD readNumBytes = numBytesToRead;
        DWORD numBytesRead = 0;
        do 
        {
            readSuccess = InternetReadFile(hRequest, ptr, readNumBytes, &numBytesRead);
            readNumBytes -= numBytesRead;
            ptr += numBytesRead;
        }
        while ((!readSuccess) && (numBytesRead > 0));
        this->responseStream->Write(buffer, numBytesToRead);
        Memory::Free(buffer);
    }
    this->responseStream->Close();
    return true;
}

} // namespace Win32