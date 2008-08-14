#pragma once
#ifndef NET_HTTPSTREAM_H
#define NET_HTTPSTREAM_H
//------------------------------------------------------------------------------
/**
    @class Net::HttpStream
  
    This wraps a request to an HTTP server into a standard stream object.
    Currently, only downloading is supported (HTTP GET), no uploads. For
    more powerful HTTP communication, use a HttpRequest object directly.
    
    Note: if many HTTP requests to the same server are executed in a
    row it is much faster to create a HttpConnection object once, open
    the connection, and set this as external HttpConnection object on
    all HttpStream and HttpRequest objects associated with this 
    connection.

    (C) 2006 Radon Labs GmbH
*/
#include "io/stream.h"
#include "http/httpconnection.h"
#include "http/httprequest.h"

//------------------------------------------------------------------------------
namespace Net
{
class HttpStream : public IO::Stream
{
    DeclareClass(HttpStream);
public:
    /// constructor
    HttpStream();
    /// destructor
    virtual ~HttpStream();

    /// set optional existing HttpConnection object
    void SetHttpConnection(HttpConnection* httpConnection);
    /// get optional existing HttpConnection object (can be 0)
    HttpConnection* GetHttpConnection() const;

    /// return true if the stream supports reading
    virtual bool CanRead() const;
    /// return true if the stream supports seeking
    virtual bool CanSeek() const;
    /// get the size of the stream in bytes
    virtual Size GetSize() const;
    /// get the current position of the read/write cursor
    virtual Position GetPosition() const;

    /// open the stream
    virtual bool Open();
    /// close the stream
    virtual void Close();
    /// directly read from the stream
    virtual Size Read(void* ptr, Size numBytes);
    /// seek in stream
    virtual void Seek(Offset offset, SeekOrigin origin);
    /// return true if end-of-stream reached
    virtual bool Eof() const;

private:
    Ptr<HttpConnection> httpConnection;
    Ptr<IO::Stream> responseStream;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
HttpStream::SetHttpConnection(HttpConnection* conn)
{
    n_assert(0 != conn);
    this->httpConnection = conn;
}

//------------------------------------------------------------------------------
/**
*/
inline
HttpConnection*
HttpStream::GetHttpConnection() const
{
    return this->httpConnection.get_unsafe();
}

} // namespace Net
//------------------------------------------------------------------------------
#endif

