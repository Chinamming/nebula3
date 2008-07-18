#pragma once
#ifndef NET_HTTPCONNECTION_H
#define NET_HTTPCONNECTION_H
//------------------------------------------------------------------------------
/**
    @class Net::HttpConnection
    
    Implements a connection to an HTTP server which allows to send 
    HttpRequests to the server and receive HttpResponses in return.
    Note that these low level classes are generally blocking. To
    get non-blocking behaviour, use the highlevel IO::Interface object
    to access a Http server through HttpStream objects.
    
    It is not a good idea to connect to and disconnect from the
    same HTTP server frequently, that's why the class
    HttpConnectionCache should be used to cache open HttpConnections
    which will be reused by HttpRequest objects.
    
    (C) 2006 Radon Labs GmbH
*/
#if __WIN32__
#include "http/win32/win32httpconnection.h"
namespace Net
{
class HttpConnection : public Win32::Win32HttpConnection
{
DeclareClass(HttpConnection);
};
}
#else
#error "IMPLEMENT ME!"
#endif
//------------------------------------------------------------------------------
#endif
