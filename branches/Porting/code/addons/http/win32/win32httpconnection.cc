//------------------------------------------------------------------------------
//  win32httpconnection.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/win32/win32httpconnection.h"

namespace Win32
{
ImplementClass(Win32::Win32HttpConnection, 'htcn', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
Win32HttpConnection::Win32HttpConnection() :
    hConnection(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Win32HttpConnection::~Win32HttpConnection()
{
    if (this->IsConnected())
    {
        this->Disconnect();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
Win32HttpConnection::Connect()
{
    if (Win32InetConnection::Connect())
    {
        n_assert((this->uri.Scheme() == "http") || (this->uri.Scheme() == "https"));
        n_assert(this->uri.Host().IsValid());
        n_assert(0 != this->hInternet);
        
        // set a suitable port number
        INTERNET_PORT portNum;
        if (this->uri.Port().IsValid())
        {
            portNum = (INTERNET_PORT) this->uri.Port().AsInt();
        }
        else if (this->uri.Scheme() == "http")
        {
            portNum = INTERNET_DEFAULT_HTTP_PORT;
        }
        else
        {
            portNum = INTERNET_DEFAULT_HTTPS_PORT;
        }
        
        // connect to HTTP server
        /// @todo user name and password!
        this->hConnection = InternetConnect(this->hInternet,                   // hInternet
                                            this->uri.Host().AsCharPtr(),      // lpszServerName
                                            portNum,                           // nServerPort
                                            0,   // lpszUserName
                                            0,   // lpszPassword
                                            INTERNET_SERVICE_HTTP,             // dwService
                                            0,                                 // dwFlags
                                            0);                                // dwContext
        if (0 == this->hConnection)
        {
            this->SetErrorToInternetLastResponse();
            Win32InetConnection::Disconnect();
            return false;
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
Win32HttpConnection::Disconnect()
{
    n_assert(this->IsConnected());
    if (0 != this->hConnection)
    {
        InternetCloseHandle(this->hConnection);
        this->hConnection = 0;
    }
    Win32InetConnection::Disconnect();
}

} // namespace Win32
