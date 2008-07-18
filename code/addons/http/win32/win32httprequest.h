#pragma once
#ifndef WIN32_WIN32HTTPREQUEST_H
#define WIN32_WIN32HTTPREQUEST_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32HttpRequest
    
    Win32 implementation of HttpRequest. Don't use this class directly,
    instead use the wrapper class Net::HttpRequest.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "io/uri.h"
#include "io/stream.h"
#include "http/httpconnection.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32HttpRequest : public Core::RefCounted
{
    DeclareClass(Win32HttpRequest);
public:
    /// allowable methods
    enum Method
    {
        Get,
        Head,
        Post,
        Put,
        Delete,
        Trace,
        Options,
    };

    /// constructor
    Win32HttpRequest();
    /// destructor
    virtual ~Win32HttpRequest();
    /// set URI for this request
    void SetURI(const IO::URI& uri);
    /// get URI for this request
    const IO::URI& GetURI() const;
    /// set optional existing HttpConnection object
    void SetHttpConnection(const Ptr<Net::HttpConnection>& httpConnection);
    /// get optional existing HttpConnection object (can be 0)
    const Ptr<Net::HttpConnection>& GetHttpConnection() const;
    /// set method to use for the request
    void SetMethod(Method m);
    /// get method for this request
    Method GetMethod() const;
    /// set optional HTTP version, the default is "HTTP/1.1"
    void SetHttpVersion(const Util::String& v);
    /// get http version
    const Util::String& GetHttpVersion() const;
    /// add an optional request header (for instance Content-Type)
    void AddRequestHeader(const Util::String& keyword, const Util::String& value);
    /// access to request headers
    const Util::Dictionary<Util::String,Util::String>& GetRequestHeaders() const;    
    /// set optional content stream
    void SetContentStream(const Ptr<IO::Stream>& stream);
    /// get optional content stream
    const Ptr<IO::Stream>& GetContentStream() const;
    /// send the request to the HTTP server, this method blocks
    bool Send();
    /// get the response of the last Send(), use HttpResponseReader class to read it
    const Ptr<IO::Stream>& GetResponse() const;
    /// get the last error message
    const Util::String& GetError() const;

private:
    /// return the method as string
    Util::String GetMethodAsString() const;
    /// set error message
    void SetError(const Util::String& err);
    /// read the HTTP-response into a stream
    bool ReadResponse(HINTERNET hRequest);
    /// sets the error message to the last internet API error
    void SetErrorToInternetLastResponse();
    /// sets additional the request headers 
    void SetAdditionalRequestHeaders(HINTERNET hRequest);

    IO::URI uri;
    Ptr<Net::HttpConnection> httpConnection;
    Method method;
    Util::String httpVersion;
    Util::Dictionary<Util::String,Util::String> requestHeaders;
    Ptr<IO::Stream> contentStream;
    Ptr<IO::Stream> responseStream;
    Util::String error;
};

//------------------------------------------------------------------------------
/**
*/
inline void
Win32HttpRequest::SetURI(const IO::URI& u)
{
    this->uri = u;
}

//------------------------------------------------------------------------------
/**
*/
inline const IO::URI&
Win32HttpRequest::GetURI() const
{
    return this->uri;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32HttpRequest::SetHttpConnection(const Ptr<Net::HttpConnection>& c)
{
    this->httpConnection = c;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Net::HttpConnection>&
Win32HttpRequest::GetHttpConnection() const
{
    return this->httpConnection;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32HttpRequest::SetMethod(Method m)
{
    this->method = m;
}

//------------------------------------------------------------------------------
/**
*/
inline Win32HttpRequest::Method
Win32HttpRequest::GetMethod() const
{
    return this->method;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32HttpRequest::SetHttpVersion(const Util::String& v)
{
    n_assert(v.IsValid());
    this->httpVersion = v;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
Win32HttpRequest::GetHttpVersion() const
{
    return this->httpVersion;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32HttpRequest::AddRequestHeader(const Util::String& keyword, const Util::String& value)
{
    this->requestHeaders.Add(keyword, value);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Dictionary<Util::String, Util::String>&
Win32HttpRequest::GetRequestHeaders() const
{
    return this->requestHeaders;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32HttpRequest::SetContentStream(const Ptr<IO::Stream>& s)
{
    this->contentStream = s;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<IO::Stream>&
Win32HttpRequest::GetContentStream() const
{
    return this->contentStream;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<IO::Stream>&
Win32HttpRequest::GetResponse() const
{
    return this->responseStream;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32HttpRequest::SetError(const Util::String& err)
{
    this->error = err;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
Win32HttpRequest::GetError() const
{
    return this->error;
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
Win32HttpRequest::GetMethodAsString() const
{
    switch (this->method)
    {
        case Head: 
            return "HEAD";
        case Post: 
            return "POST";
        case Put: 
            return "PUT";
        case Delete: 
            return "DELETE";
        case Trace: 
            return "TRACE";
        case Options: 
            return "OPTIONS";
        case Get: 
        default:            
            return "GET";
    }
}

} // namespace Win32
//------------------------------------------------------------------------------
#endif


