//------------------------------------------------------------------------------
//  httpstream.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httpstream.h"

namespace Net
{
ImplementClass(Net::HttpStream, 'HTST', IO::Stream);

using namespace IO;

//------------------------------------------------------------------------------
/**
*/
HttpStream::HttpStream()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
HttpStream::~HttpStream()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
HttpStream::CanRead() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
HttpStream::CanSeek() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
    This creates a GET HttpRequest and stores the response stream internally.
*/
bool
HttpStream::Open()
{
    n_assert(!this->IsOpen());
    n_assert(this->uri.Scheme() == "http");
    n_assert(!this->responseStream.isvalid());
    if (Stream::Open())
    {
        Ptr<HttpRequest> httpRequest = HttpRequest::Create();
        httpRequest->SetURI(this->uri);
        httpRequest->SetMethod(HttpRequest::Get);
        if (this->httpConnection.isvalid())
        {
            httpRequest->SetHttpConnection(this->httpConnection);
        }
        if (httpRequest->Send())
        {
            this->responseStream = httpRequest->GetResponse();
            if (this->responseStream.isvalid())
            {
                this->responseStream->SetAccessMode(Stream::ReadAccess);
                this->responseStream->Open();
            }
            return true;
        }
    }

    // fallthrough: failure
    Stream::Close();
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpStream::Close()
{
    n_assert(this->IsOpen());
    this->responseStream = 0;
    Stream::Close();
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
HttpStream::GetSize() const
{
    n_assert(this->IsOpen());
    if (this->responseStream.isvalid())
    {
        return this->responseStream->GetSize();
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
Stream::Position
HttpStream::GetPosition() const
{
    n_assert(this->IsOpen());
    if (this->responseStream.isvalid())
    {
        return this->responseStream->GetPosition();
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
HttpStream::Eof() const
{
    n_assert(this->IsOpen());
    if (this->responseStream.isvalid())
    {
        return this->responseStream->Eof();
    }
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
HttpStream::Read(void* ptr, Size numBytes)
{
    n_assert(this->IsOpen());
    if (this->responseStream.isvalid())
    {
        return this->responseStream->Read(ptr, numBytes);
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
HttpStream::Seek(Offset offset, SeekOrigin origin)
{
    n_assert(this->IsOpen());
    if (this->responseStream.isvalid())
    {
        this->responseStream->Seek(offset, origin);
    }
}

} // namespace Net

