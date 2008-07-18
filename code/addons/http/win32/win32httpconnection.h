#pragma once
#ifndef WIN32_WIN32HTTPCONNECTION_H
#define WIN32_WIN32HTTPCONNECTION_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32HttpConnection
  
    Manages a connection to a HTTP server. This is the Win32
    specific implementation which wraps around Win32's 
    "Windows Internet" API. Note that these low level classes
    generally block. To get non-blocking behaviour use the
    high level HttpStream class together with the 
    asynchronous IO::Interface.

    (C) 2006 Radon Labs GmbH
*/
#include "http/win32/win32inetconnection.h"
#include "io/uri.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32HttpConnection : public Win32InetConnection
{
    DeclareClass(Win32HttpConnection);
public:
    /// constructor
    Win32HttpConnection();
    /// destructor
    virtual ~Win32HttpConnection();
    /// connect to HTTP server
    virtual bool Connect();
    /// disconnect from HTTP server
    virtual void Disconnect();

private:
    friend class Win32HttpRequest;
    /// get the internal connection handle
    HINTERNET GetConnectionHandle() const;

    HINTERNET hConnection;
};

//------------------------------------------------------------------------------
/**
*/
inline
HINTERNET
Win32HttpConnection::GetConnectionHandle() const
{
    return this->hConnection;
}

} // namespace Win32
//------------------------------------------------------------------------------
#endif
