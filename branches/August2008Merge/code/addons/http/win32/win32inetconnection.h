#pragma once
#ifndef WIN32_WIN32INETCONNECTION_H
#define WIN32_WIN32INETCONNECTION_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32InetConnection

    Base class for connections to the internet through HTTP and FTP
    protocols. Wraps around the "Windows Internet" API.

    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "io/uri.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32InetConnection : public Core::RefCounted
{
    DeclareClass(Win32InetConnection);
public:
    /// constructor
    Win32InetConnection();
    /// destructor
    virtual ~Win32InetConnection();
    /// set URI of connection
    void SetURI(const IO::URI& uri);
    /// get URI of connection
    const IO::URI& GetURI() const;
    /// set agent name
    void SetAgent(const Util::String& a);
    /// get agent name
    const Util::String& GetAgent() const;
    /// connect to internet
    virtual bool Connect();
    /// disconnect from internet
    virtual void Disconnect();
    /// return true if currently connected
    bool IsConnected() const;
    /// get error string if connection has failed
    const Util::String& GetError() const;

protected:
    /// set error string
    void SetError(const Util::String& s);
    /// do dialup if necessary
    bool DoDialupStuff();
    /// set error to the result of InternetGetLastResponseInfo()
    void SetErrorToInternetLastResponse();

    IO::URI uri;
    Util::String agent;
    HINTERNET hInternet;
    bool isConnected;
    Util::String error;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
Win32InetConnection::SetURI(const IO::URI& u)
{
    this->uri = u;
}

//------------------------------------------------------------------------------
/**
*/
inline
const IO::URI&
Win32InetConnection::GetURI() const
{
    return this->uri;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Win32InetConnection::SetAgent(const Util::String& a)
{
    n_assert(a.IsValid());
    this->agent = a;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
Win32InetConnection::GetAgent() const
{
    return this->agent;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
Win32InetConnection::IsConnected() const
{
    return this->isConnected;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Win32InetConnection::SetError(const Util::String& s)
{
    this->error = s;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
Win32InetConnection::GetError() const
{
    return this->error;
}

} // namespace Win32
//------------------------------------------------------------------------------
#endif

