#pragma once
#ifndef NET_HTTPREQUEST_H
#define NET_HTTPREQUEST_H
//------------------------------------------------------------------------------
/**
    @class Net::HttpRequest
  
    The HttpRequest class implements a HTTP request to an HTTP server.
    If a HttpConnection object is provided, this connection is used,
    otherwise, the HttpRequest object will create its own HttpConnection
    object. If many HttpRequests are sent to the same server it is
    better to create an HttpRequest object before-hand and re-use
    this for all HttpRequests. This provides better performance then
    opening a separate connection for each request.
    
    (C) 2006 Radon Labs GmbH
*/
#if __WIN32__
#include "http/win32/win32httprequest.h"
namespace Net
{
class HttpRequest : public Win32::Win32HttpRequest
{
DeclareClass(HttpRequest);
};
}
#else
#error "IMPLEMENT ME!"
#endif
//------------------------------------------------------------------------------
#endif
